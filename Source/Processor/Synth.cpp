#include "Synth.h"
#include "AuxDsp.h"
#include "Globals.h"
#include "LFO.h"
#include "PolyOscillator.h"
#include "WaveTableOscillator.h"
#include "juce_audio_processors/juce_audio_processors.h"

Synth::Synth(juce::AudioProcessorValueTreeState &apvts)
    : apvts(apvts), polyOscillator(MIDI_NOTES),
      waveTableOscillator(WAVE_TABLE_SIZE), lfoA(WAVE_TABLE_SIZE),
      lfoF(WAVE_TABLE_SIZE) {
    // define global variables if needed
    Globals::lfoF = &lfoF;
    // Flag critical parameters
    flagCriticalParameters();

    //  Add listener to critical parameters
    //  Volume
    /*apvts.addParameterListener("Volume", this);*/
    apvts.addParameterListener("WaveTableVolume1", this);
    apvts.addParameterListener("WaveTableVolume2", this);

    // WaveTablePlaying
    apvts.addParameterListener("WaveTablePlaying1", this);
    apvts.addParameterListener("WaveTablePlaying2", this);

    // Additive osccilator 1 and 2 params
    for (int i = 1; i <= NUM_HARMONICS; i++) {
        apvts.addParameterListener("Oscillator1A" + std::to_string(i), this);
        apvts.addParameterListener("Oscillator1P" + std::to_string(i), this);

        apvts.addParameterListener("Oscillator2A" + std::to_string(i), this);
        apvts.addParameterListener("Oscillator2P" + std::to_string(i), this);
    }

    // Oscillators waveform
    apvts.addParameterListener("WaveTableType1", this);
    apvts.addParameterListener("WaveTableType2", this);

    // LFO Amplitude
    apvts.addParameterListener("WaveTableTypeLFOA", this);
    apvts.addParameterListener("LFOAPlaying", this);

    // LFO Frequency
    apvts.addParameterListener("WaveTableTypeLFOF", this);
    apvts.addParameterListener("LFOFPlaying", this);

    // Delay parameters
    apvts.addParameterListener("DelayPlaying", this);
}

Synth::~Synth() {}

void Synth::prepareToPlay(double sampleRate) {
    Globals::sampleRate = sampleRate;
}

void Synth::processBlock(juce::AudioBuffer<float> &buffer,
                         juce::MidiBuffer &midiMessages) {
    // Get all parameters from the ValueTree
    updateParameters();
    updateCriticalParameters();
    // filter.updateFilter(1000 / (sampleRate * 0.5), 0.0f, "lp", "hamming");

    int currentSample = 0;

    for (juce::MidiMessageMetadata midiMetadata : midiMessages) {
        juce::MidiMessage message = midiMetadata.getMessage();
        int messagePosition = static_cast<int>(message.getTimeStamp());

        render(buffer, currentSample, messagePosition);
        currentSample = messagePosition;
        handleMidiEvent(message);
    }

    if (polyOscillator.isProcessing() || delay.isProcessing())
        render(buffer, currentSample, buffer.getNumSamples());
}

void Synth::handleMidiEvent(juce::MidiMessage &midiMessage) {
    if (midiMessage.isNoteOn()) {
        polyOscillator.playOscillator(
            midiMessage.getNoteNumber(), WAVE_TABLE_SIZE,
            {parameters.attackA, parameters.decayA, parameters.sustainA,
             parameters.releaseA});
    } else if (midiMessage.isNoteOff()) {
        polyOscillator.stopOscillator(midiMessage.getNoteNumber());
    } else if (midiMessage.isAllNotesOff()) {
        for (int i = 0; i < MIDI_NOTES; i++) {
            polyOscillator.stopOscillator(i);
        }
    }
}

void Synth::render(juce::AudioBuffer<float> &buffer, int beginSample,
                   int endSample) {
    float *firstChannel = buffer.getWritePointer(0);
    float *secondChannel = buffer.getWritePointer(1);

    for (int sample = beginSample; sample < endSample; ++sample) {
        float volume = parameters.volume;
        float value = 0.0f;
        // Get sample
        value = polyOscillator.getNextSample(waveTableOscillator.getWaveTable(),
                                             parameters.noise);
        // If voices not are playing, LFOA and LFOF are reseted and are not pr
        // ocessed
        const bool polyOscillatorIsPlayingVoices =
            polyOscillator.isProcessing();

        if (polyOscillatorIsPlayingVoices) {

            if (lfoA.isPlaying()) {
                lfoA.nextSample();
                volume = lfoA.getLFOSample();
            }

            if (lfoF.isPlaying()) {
                lfoF.nextSample();
            }
        } else {
            lfoA.resetIndex();
            lfoF.resetIndex();
        }

        // Iterate delay
        if (delay.isPlaying()) {
            value = delay.getDelaySample(value);
        }

        if (filter.isPlaying())
            value = filter.process(value);

        value *= volume;

        firstChannel[sample] = value;
        secondChannel[sample] = value;
    }
}

void Synth::updateParameters() {
    // Volume
    parameters.volume = myDsp::dbToLinearZeroMinus60(
        apvts.getRawParameterValue("Volume")->load());

    // Noise
    parameters.noise =
        static_cast<juce::AudioParameterBool *>(apvts.getParameter("Noise"))
            ->get();

    // Amplidute envelope
    parameters.attackA = apvts.getRawParameterValue("AttackA")->load();
    parameters.decayA = apvts.getRawParameterValue("DecayA")->load();
    parameters.sustainA = apvts.getRawParameterValue("SustainA")->load();
    parameters.releaseA = apvts.getRawParameterValue("ReleaseA")->load();

    // LFO Amplitude parameters
    parameters.frequencyLFOA =
        apvts.getRawParameterValue("FrequencyLFOA")->load();
    parameters.amountLFOA = apvts.getRawParameterValue("AmountLFOA")->load();

    lfoA.setLFOParameters(parameters.frequencyLFOA,
                          parameters.volume -
                              parameters.amountLFOA * parameters.volume,
                          parameters.volume);

    // LFO Frequency parameters
    parameters.frequencyLFOF =
        apvts.getRawParameterValue("FrequencyLFOF")->load();
    parameters.centsLFOF = apvts.getRawParameterValue("CentsLFOF")->load();

    lfoF.setLFOParameters(parameters.frequencyLFOF, -parameters.centsLFOF,
                          parameters.centsLFOF);

    // Delay parameters
    parameters.delayTime = apvts.getRawParameterValue("DelayTime")->load();
    parameters.delayFeedback = myDsp::dbToLinearZeroMinus60(
        apvts.getRawParameterValue("DelayFeedback")->load());
    parameters.delayWet = myDsp::dbToLinearZeroMinus60(
        apvts.getRawParameterValue("DelayWet")->load());
    parameters.delayDry = myDsp::dbToLinearZeroMinus60(
        apvts.getRawParameterValue("DelayDry")->load());

    delay.setDelayParameters(parameters.delayTime, parameters.delayFeedback,
                             parameters.delayWet, parameters.delayDry);

    // Filter parameters
    parameters.filterType = static_cast<myDsp::BiquadFilter::FilterType>(
        static_cast<juce::AudioParameterChoice *>(
            apvts.getParameter("FilterType"))
            ->getIndex());
    parameters.frequencyFilter =
        apvts.getRawParameterValue("FilterFrequency")->load();
    parameters.qFilter = apvts.getRawParameterValue("FilterQ")->load();
    parameters.filterPlaying = static_cast<juce::AudioParameterBool *>(
                                   apvts.getParameter("FilterPlaying"))
                                   ->get();

    if (parameters.filterPlaying)
        filter.play();
    else
        filter.stop();

    filter.setFilter(parameters.filterType, parameters.frequencyFilter,
                     parameters.qFilter);
}

void Synth::updateCriticalParameters() {
    // Critical section juce
    /*const juce::ScopedLock lock(criticalSection);*/
    // Update main waveTalbe if needed
    bool updateMainWaveTable = false;

    // waveTable parameters
    if (changeStateCriticalParameters.waveTableVolume1.get()) {
        parameters.waveTableVolume1 = myDsp::dbToLinearZeroMinus60(
            apvts.getRawParameterValue("WaveTableVolume1")->load());
        changeStateCriticalParameters.waveTableVolume1.set(false);
        updateMainWaveTable = true;
    }

    if (changeStateCriticalParameters.waveTableVolume2.get()) {
        parameters.waveTableVolume2 = myDsp::dbToLinearZeroMinus60(
            apvts.getRawParameterValue("WaveTableVolume2")->load());
        changeStateCriticalParameters.waveTableVolume2.set(false);
        updateMainWaveTable = true;
    }

    if (changeStateCriticalParameters.waveTablePlaying1.get()) {
        parameters.waveTablePlaying1 =
            static_cast<juce::AudioParameterBool *>(
                apvts.getParameter("WaveTablePlaying1"))
                ->get();
        changeStateCriticalParameters.waveTablePlaying1.set(false);
        updateMainWaveTable = true;
    }

    if (changeStateCriticalParameters.waveTablePlaying2.get()) {
        parameters.waveTablePlaying2 =
            static_cast<juce::AudioParameterBool *>(
                apvts.getParameter("WaveTablePlaying2"))
                ->get();
        changeStateCriticalParameters.waveTablePlaying2.set(false);
        updateMainWaveTable = true;
    }

    if (changeStateCriticalParameters.waveTableType1.get() ||
        changeStateCriticalParameters.oscillator1parameters.get()) {

        if (changeStateCriticalParameters.waveTableType1.get()) {
            parameters.waveTableType1 =
                static_cast<WaveTableOscillator::WaveTableType>(
                    static_cast<juce::AudioParameterChoice *>(
                        apvts.getParameter("WaveTableType1"))
                        ->getIndex());
            changeStateCriticalParameters.waveTableType1.set(false);
        }

        if (changeStateCriticalParameters.oscillator1parameters.get()) {
            for (int i = 1; i <= NUM_HARMONICS; i++) {
                parameters.oscillator1[i - 1].magnitude =
                    apvts
                        .getRawParameterValue("Oscillator1A" +
                                              std::to_string(i))
                        ->load();
                parameters.oscillator1[i - 1].phase =
                    apvts
                        .getRawParameterValue("Oscillator1P" +
                                              std::to_string(i))
                        ->load();
            }
            changeStateCriticalParameters.oscillator1parameters.set(false);
        }

        waveTableOscillator.setWaveTable(0, parameters.waveTableType1,
                                         parameters.oscillator1);
        updateMainWaveTable = true;
    }

    if (changeStateCriticalParameters.waveTableType2.get() ||
        changeStateCriticalParameters.oscillator2parameters.get()) {

        if (changeStateCriticalParameters.waveTableType2.get()) {
            parameters.waveTableType2 =
                static_cast<WaveTableOscillator::WaveTableType>(
                    static_cast<juce::AudioParameterChoice *>(
                        apvts.getParameter("WaveTableType2"))
                        ->getIndex());
            changeStateCriticalParameters.waveTableType2.set(false);
        }

        if (changeStateCriticalParameters.oscillator2parameters.get()) {
            for (int i = 1; i <= NUM_HARMONICS; i++) {
                parameters.oscillator2[i - 1].magnitude =
                    apvts
                        .getRawParameterValue("Oscillator2A" +
                                              std::to_string(i))
                        ->load();
                parameters.oscillator2[i - 1].phase =
                    apvts
                        .getRawParameterValue("Oscillator2P" +
                                              std::to_string(i))
                        ->load();
            }
            changeStateCriticalParameters.oscillator2parameters.set(false);
        }

        waveTableOscillator.setWaveTable(1, parameters.waveTableType2,
                                         parameters.oscillator2);
        updateMainWaveTable = true;
    }

    if (updateMainWaveTable) {
        waveTableOscillator.updateMainWaveTable(
            {parameters.waveTablePlaying1, parameters.waveTablePlaying2},
            {parameters.waveTableVolume1, parameters.waveTableVolume2});
    }

    // LFO Amplitude WaveTable
    if (changeStateCriticalParameters.waveTableTypeLFOA.get()) {
        parameters.waveTableTypeLFOA = static_cast<LFO::WaveTableType>(
            static_cast<juce::AudioParameterChoice *>(
                apvts.getParameter("WaveTableTypeLFOA"))
                ->getIndex());
        lfoA.setWaveTable(parameters.waveTableTypeLFOA);
        changeStateCriticalParameters.waveTableTypeLFOA.set(false);
    }

    // LFO Amplitude is playing
    if (changeStateCriticalParameters.lfoAPlaying.get()) {
        parameters.lfoAPlaying = static_cast<juce::AudioParameterBool *>(
                                     apvts.getParameter("LFOAPlaying"))
                                     ->get();
        if (parameters.lfoAPlaying) {
            lfoA.play();
        } else {
            lfoA.stop();
        }
        changeStateCriticalParameters.lfoAPlaying.set(false);
    }

    // LFO Frequency WaveTable
    if (changeStateCriticalParameters.waveTableTypeLFOF.get()) {
        parameters.waveTableTypeLFOF = static_cast<LFO::WaveTableType>(
            static_cast<juce::AudioParameterChoice *>(
                apvts.getParameter("WaveTableTypeLFOF"))
                ->getIndex());
        lfoF.setWaveTable(parameters.waveTableTypeLFOF);
        changeStateCriticalParameters.waveTableTypeLFOF.set(false);
    }

    // LFO Frequency is playing
    if (changeStateCriticalParameters.lfoFPlaying.get()) {
        parameters.lfoFPlaying = static_cast<juce::AudioParameterBool *>(
                                     apvts.getParameter("LFOFPlaying"))
                                     ->get();
        if (parameters.lfoFPlaying) {
            lfoF.play();
        } else {
            lfoF.stop();
        }
        changeStateCriticalParameters.lfoFPlaying.set(false);
    }

    // Delay is playing
    if (changeStateCriticalParameters.delayPlaying.get()) {
        parameters.delayPlaying = static_cast<juce::AudioParameterBool *>(
                                      apvts.getParameter("DelayPlaying"))
                                      ->get();
        if (parameters.delayPlaying) {
            delay.play();
        } else {
            delay.stop();
        }
        changeStateCriticalParameters.delayPlaying.set(false);
    }
}

void Synth::parameterChanged(const juce::String &parameterID, float newValue) {
    const std::string parameterIDString = parameterID.toStdString();

    if (parameterIDString.find("Oscillator1") != std::string::npos) {
        changeStateCriticalParameters.oscillator1parameters.set(true);

    } else if (parameterIDString.find("Oscillator2") != std::string::npos) {
        changeStateCriticalParameters.oscillator2parameters.set(true);

    } else if (parameterID == "WaveTableType1") {
        changeStateCriticalParameters.waveTableType1.set(true);

    } else if (parameterID == "WaveTableType2") {
        changeStateCriticalParameters.waveTableType2.set(true);

    } else if (parameterID == "WaveTablePlaying1") {
        changeStateCriticalParameters.waveTablePlaying1.set(true);

    } else if (parameterID == "WaveTablePlaying2") {
        changeStateCriticalParameters.waveTablePlaying2.set(true);

    } else if (parameterID == "WaveTableVolume1") {
        changeStateCriticalParameters.waveTableVolume1.set(true);

    } else if (parameterID == "WaveTableVolume2") {
        changeStateCriticalParameters.waveTableVolume2.set(true);

    } else if (parameterID == "WaveTableTypeLFOA") {
        changeStateCriticalParameters.waveTableTypeLFOA.set(true);

    } else if (parameterID == "LFOAPlaying") {
        changeStateCriticalParameters.lfoAPlaying.set(true);

    } else if (parameterID == "WaveTableTypeLFOF") {
        changeStateCriticalParameters.waveTableTypeLFOF.set(true);

    } else if (parameterID == "LFOFPlaying") {
        changeStateCriticalParameters.lfoFPlaying.set(true);

    } else if (parameterID == "DelayPlaying") {
        changeStateCriticalParameters.delayPlaying.set(true);
    }
}

void Synth::flagCriticalParameters() {

    changeStateCriticalParameters.oscillator1parameters.set(true);
    changeStateCriticalParameters.oscillator2parameters.set(true);
    changeStateCriticalParameters.waveTableType1.set(true);
    changeStateCriticalParameters.waveTableType2.set(true);
    changeStateCriticalParameters.waveTablePlaying1.set(true);
    changeStateCriticalParameters.waveTablePlaying2.set(true);
    changeStateCriticalParameters.waveTableVolume1.set(true);
    changeStateCriticalParameters.waveTableVolume2.set(true);

    // LFO Amplitude
    changeStateCriticalParameters.waveTableTypeLFOA.set(true);
    changeStateCriticalParameters.lfoAPlaying.set(true);

    // LFO Frequency
    changeStateCriticalParameters.waveTableTypeLFOF.set(true);
    changeStateCriticalParameters.lfoFPlaying.set(true);

    // Delay parameters
    changeStateCriticalParameters.delayPlaying.set(true);
}
