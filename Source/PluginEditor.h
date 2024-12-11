/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "Editor/MyLookAndFeel.h"
#include "PluginProcessor.h"
#include "Editor/GeneralVolumeEditor.cpp"
#include "Editor/OscillatorEditor.cpp"
#include "Editor/NoiseEditor.cpp"
#include "Editor/EnvelopeEditor.cpp"
#include "Editor/LFOAmplitudeEditor.cpp"
#include "Editor/LFOFrequencyEditor.cpp"
#include "Editor/DelayEditor.cpp"
#include "Editor/FilterEditor.cpp"
#include "Editor/LogoEditor.cpp"

#include <JuceHeader.h>

//==============================================================================
/**
 */
class SoftSynthAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    SoftSynthAudioProcessorEditor(SoftSynthAudioProcessor &);
    ~SoftSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoftSynthAudioProcessor &audioProcessor;
    MyLookAndFeel lookAndFeel;
    GeneralVolumeEditor generalVolume;
    OscillatorEditor oscillator1;
    OscillatorEditor oscillator2;
    LFOFrequencyEditor lfoFrequency;
    NoiseEditor noise;
    EnvelopeEditor envelope;
    LFOAmplitudeEditor lfoAmplitude;
    DelayEditor delay;
    FilterEditor filter;
    LogoEditor logo;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoftSynthAudioProcessorEditor)
};
