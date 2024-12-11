#pragma once
#include "AuxDsp.h"
#include "Globals.h"
#include "LFO.h"
#include "Delay.h"
#include "PolyOscillator.h"
#include "WaveTableOscillator.h"
#include <JuceHeader.h>

#define MIDI_NOTES 128
#define WAVE_TABLE_SIZE 1024
#define NUM_HARMONICS 12

class Synth : public juce::AudioProcessorValueTreeState::Listener {
public:
    // Synth constructor with AudioProcessorValueTreeState as parameter
    // Parameters: juce::AudioProcessorValueTreeState &apvts
    Synth(juce::AudioProcessorValueTreeState &apvts);
    ~Synth();

    // Prepare to play function use to set the sample rate
    // Parameters: double sampleRate
    // Returns: void
    void prepareToPlay(double sampleRate);
    // Process block function to process audio and midi messages
    // Parameters: juce::AudioBuffer<float> &buffer,
    // juce::MidiBuffer &midiMessages
    // Returns: void
    void processBlock(juce::AudioBuffer<float> &buffer,
                      juce::MidiBuffer &midiMessages);

    void flagCriticalParameters();

private:
    // ValueTree reference
    juce::AudioProcessorValueTreeState &apvts;

    // Struct with all paramaters whose change will need high computational cost
    struct ChangeStateCriticalParameters {
        // WaveTableOscillators
        juce::Atomic<bool> oscillator1parameters;
        juce::Atomic<bool> oscillator2parameters;
        juce::Atomic<bool> waveTableType1;
        juce::Atomic<bool> waveTableType2;
        juce::Atomic<bool> waveTablePlaying1;
        juce::Atomic<bool> waveTablePlaying2;
        juce::Atomic<bool> waveTableVolume1;
        juce::Atomic<bool> waveTableVolume2;

        // LFO Amplitude
        juce::Atomic<bool> waveTableTypeLFOA;
        juce::Atomic<bool> lfoAPlaying;

        // LFO Frequency
        juce::Atomic<bool> waveTableTypeLFOF;
        juce::Atomic<bool> lfoFPlaying;

        // Delay parameters
        juce::Atomic<bool> delayPlaying;

    } changeStateCriticalParameters;

    // Struct with all parameters
    struct Parameters {
        // Volumes
        float volume;
        float waveTableVolume1;
        float waveTableVolume2;

        // Oscillators waveTables
        bool waveTablePlaying1;
        bool waveTablePlaying2;
        std::vector<Pair> oscillator1{NUM_HARMONICS};
        std::vector<Pair> oscillator2{NUM_HARMONICS};
        WaveTableOscillator::WaveTableType waveTableType1;
        WaveTableOscillator::WaveTableType waveTableType2;

        // Noise
        bool noise;

        // Amplidute envelope
        float attackA;
        float decayA;
        float sustainA;
        float releaseA;

        // LFO Amplitude
        float frequencyLFOA;
        LFO::WaveTableType waveTableTypeLFOA;
        float amountLFOA;
        bool lfoAPlaying;

        // LFO Frequency
        float frequencyLFOF;
        LFO::WaveTableType waveTableTypeLFOF;
        float centsLFOF;
        bool lfoFPlaying;

        // Delay parameters
        float delayTime;
        float delayFeedback;
        float delayWet;
        float delayDry;
        float delayPlaying;

        // Filter parameters
        myDsp::BiquadFilter::FilterType filterType;
        float frequencyFilter;
        float qFilter;
        bool filterPlaying;

    } parameters;

    // Synth components
    // Oscillators
    PolyOscillator polyOscillator;

    // WaveTableOscillators
    WaveTableOscillator waveTableOscillator;

    // LFO Amplitude
    LFO lfoA;

    // LFO Frequency
    LFO lfoF;

    // Delay
    Delay delay;

    // Filters
    myDsp::BiquadFilter filter;

    // Handel midi events
    // Parameters: juce::MidiMessage &midiMessage
    // Returns: void
    void handleMidiEvent(juce::MidiMessage &midiMessage);

    // Render auido block from beginSample to endSample
    // Parameters: juce::AudioBuffer<float> &buffer, int beginSample,
    //            int endSample
    // Returns: void
    void render(juce::AudioBuffer<float> &buffer, int beginSample,
                int endSample);

    // Handle parameter changes
    // Change parameters whose change will no need high computational cost
    // Parameters: void
    // Returns: void
    void updateParameters();

    // Hancle parameter that will need high computational cost
    // Parameters: void
    // Returns: void
    void updateCriticalParameters();

    // Callback function for parameter changes that will neeed high
    // computational cost
    // Parameters: const juce::String &parameterID, float newValue
    // Returns: void
    void parameterChanged(const juce::String &parameterID,
                          float newValue) override;

};
