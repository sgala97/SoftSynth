/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Processor/Synth.h"
#include <memory>

//==============================================================================
SoftSynthAudioProcessor::SoftSynthAudioProcessor()
    :
#ifndef JucePlugin_PreferredChannelConfigurations
      AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
#endif
      apvts{*this, nullptr, "parameters", createParameterLayout()},
      synth(apvts) {
}

SoftSynthAudioProcessor::~SoftSynthAudioProcessor() {}

//==============================================================================
#ifndef JucePlugin_Name
#define JucePlugin_Name "SoftSynth"
#endif

const juce::String SoftSynthAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool SoftSynthAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SoftSynthAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SoftSynthAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SoftSynthAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int SoftSynthAudioProcessor::getNumPrograms() {
    return 1; // NB: some hosts don't cope very well if you tell them there are
              // 0 programs, so this should be at least 1, even if you're not
              // really implementing programs.
}

int SoftSynthAudioProcessor::getCurrentProgram() { return 0; }

void SoftSynthAudioProcessor::setCurrentProgram(int index) {}

const juce::String SoftSynthAudioProcessor::getProgramName(int index) {
    return {};
}

void SoftSynthAudioProcessor::changeProgramName(int index,
                                                const juce::String &newName) {}

//==============================================================================
void SoftSynthAudioProcessor::prepareToPlay(double sampleRate,
                                            int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.prepareToPlay(sampleRate);
}

void SoftSynthAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SoftSynthAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void SoftSynthAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                           juce::MidiBuffer &midiMessages) {

    juce::ScopedNoDenormals noDenormals;
    buffer.clear();
    synth.processBlock(buffer, midiMessages);
}
//==============================================================================
bool SoftSynthAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *SoftSynthAudioProcessor::createEditor() {
    // return new SoftSynthAudioProcessorEditor(*this);
    return new SoftSynthAudioProcessorEditor(*this);
}

//==============================================================================
void SoftSynthAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void SoftSynthAudioProcessor::setStateInformation(const void *data,
                                                  int sizeInBytes) {
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) {
        apvts.replaceState(tree);
        // To do: update synth parameters
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new SoftSynthAudioProcessor();
}

// Parametros

juce::AudioProcessorValueTreeState::ParameterLayout
SoftSynthAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // General Volume
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "Volume", "Volume", -60.f, 6.0f, -10.0f));

    // Volume waveTableOscillator1
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "WaveTableVolume1", "Volume", -60.0f, 6.0f, -10.0f));

    // Volume waveTableOscillator2
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "WaveTableVolume2", "Volume", -60.0f, 6.0f, -10.0f));

    // WaveTableOscillator 1 playing
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "WaveTablePlaying1", "WaveTablePlaying1", true));

    // WaveTableOscillator 2 playing
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "WaveTablePlaying2", "WaveTablePlaying2", false));

    // Oscillator 1
    for (int i = 1; i <= NUM_HARMONICS; i++) {
        // Oscillator 1
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            "Oscillator1A" + std::to_string(i), std::to_string(i), 0, 1,
            0));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            "Oscillator1P" + std::to_string(i), std::to_string(i) + "P", 0, 1,
            0));
    }

    // Oscillator 2
    for (int i = 1; i <= NUM_HARMONICS; i++) {
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            "Oscillator2A" + std::to_string(i), std::to_string(i), 0, 1,
            0));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            "Oscillator2P" + std::to_string(i), std::to_string(i) + "P", 0, 1,
            0));
    }

    // Oscillator 1 waveform
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "WaveTableType1", "Waveform",
        juce::StringArray{"custom", "sine", "square", "saw", "triangle"}, 1));

    // Oscillator 2 waveform
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "WaveTableType2", "Waveform",
        juce::StringArray{"custom", "sine", "square", "saw", "triangle"}, 1));

    // bool noisePlaying;
    layout.add(
        std::make_unique<juce::AudioParameterBool>("Noise", "Noise", false));

    // Amplitude envelope parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "AttackA", "Attack",
        juce::NormalisableRange<float>(0.001f, 10.f, 0.f, 0.300912f), 0.01f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "DecayA", "Decay",
        juce::NormalisableRange<float>(0.001f, 10.f, 0.f, 0.300912f), 1.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "SustainA", "Sustain",
        juce::NormalisableRange<float>(0.f, 1.f, 0.f, 0.575717f), 0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "ReleaseA", "Release",
        juce::NormalisableRange<float>(0.001f, 10.f, 0.f, 0.300912f), 0.01f));

    // Amplitude LFO parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "FrequencyLFOA", "Frequency",
        juce::NormalisableRange<float>(0.05f, 100.f, 0.f, 0.196757f), 1));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "WaveTableTypeLFOA", "Waveform",
        juce::StringArray{"sine", "square", "saw", "triangle"}, 0));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "AmountLFOA", "Amount", 0.f, 1.f, 0.3f));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        "LFOAPlaying", "LFOAPlaying", false));

    // Frequency LFO parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "FrequencyLFOF", "Frequency",
        juce::NormalisableRange<float>(0.05f, 100.f, 0.f, 0.196757f), 1));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "WaveTableTypeLFOF", "Waveform",
        juce::StringArray{"sine", "square", "saw", "triangle"}, 0));

    layout.add(std::make_unique<juce::AudioParameterFloat>("CentsLFOF", "Cents",
                                                           0.f, 1000.f, 20.f));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        "LFOFPlaying", "LFOFPlaying", false));

    // Delay parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "DelayTime", "Time", 0.01f, 4.0f, 0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "DelayFeedback", "Feedback", -60.f, 0.0f, -10.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "DelayWet", "Wet", -60.0f, 6.0f, 0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "DelayDry", "Dry", -60.0f, 6.0f, 0.f));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        "DelayPlaying", "DelayPlaying", false));

    // Filter parameters
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "FilterType", "Filter Type",
        juce::StringArray{"LowPass", "HighPass", "BandPass"}, 0));
    // Frequency of the filter from 20 to 20000 Hz with skew factor of 0.25f.
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "FilterFrequency", "Frequency",
        juce::NormalisableRange<float>(20.f, 20000.f, 0.f, 0.25f), 20000.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FilterQ", "Q Factor",
                                                           0.1f, 10.f, 0.8f));
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "FilterPlaying", "FilterPlaying", false));

    return layout;
}
