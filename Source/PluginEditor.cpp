/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

//==============================================================================
SoftSynthAudioProcessorEditor::SoftSynthAudioProcessorEditor(
    SoftSynthAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p),
      generalVolume(audioProcessor.apvts), oscillator1(audioProcessor.apvts, 1),
      oscillator2(audioProcessor.apvts, 2),
      lfoFrequency(audioProcessor.apvts),
      noise(audioProcessor.apvts, {&oscillator1, &oscillator2, &lfoFrequency}),
      envelope(audioProcessor.apvts), lfoAmplitude(audioProcessor.apvts),
      delay(audioProcessor.apvts), filter(audioProcessor.apvts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1200, 655);
    setResizeLimits(getWidth(), getHeight(), getWidth(), getHeight());

    // Set the look and feel
    setLookAndFeel(&lookAndFeel);

    // Add components
    addAndMakeVisible(generalVolume);
    addAndMakeVisible(oscillator1);
    addAndMakeVisible(oscillator2);
    addAndMakeVisible(noise);
    addAndMakeVisible(envelope);
    addAndMakeVisible(lfoAmplitude);
    addAndMakeVisible(lfoFrequency);
    addAndMakeVisible(delay);
    addAndMakeVisible(filter);
    addAndMakeVisible(logo);
}

SoftSynthAudioProcessorEditor::~SoftSynthAudioProcessorEditor() {
    setLookAndFeel(nullptr);
}

//==============================================================================
void SoftSynthAudioProcessorEditor::paint(juce::Graphics &g) {
    // (Our component is opaque, so we must completely fill the background with
    // a solid colour)
    g.fillAll(juce::Colour(BACKGROUND_COLOUR));
}

void SoftSynthAudioProcessorEditor::resized() {
    auto bounds = getLocalBounds();
    generalVolume.setBounds({0, 0, 200, 175});
    oscillator1.setBounds({200 - 15, 0, 1000 + 15, 175});
    noise.setBounds({0, 175 - 15, 200, 175});
    oscillator2.setBounds({200 - 15, 175 - 15, 1000 + 15, 175});
    envelope.setBounds({0, 350 - 30, 400, 175});
    lfoAmplitude.setBounds({400 - 15, 350 - 30, 400 + 15, 175});
    lfoFrequency.setBounds({800 - 15, 350 - 30, 400 + 15, 175});
    delay.setBounds({0, 525 - 45, 500, 175});
    filter.setBounds({500 - 15, 525 - 45, 400 + 15, 175});
    logo.setBounds({900 - 15, 525 - 45, 300 + 15, 175});

}
