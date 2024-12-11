#include "MyRotatorySlider.h"
#include "Colours.h"
#include <JuceHeader.h>

MyRotatorySlider::MyRotatorySlider(juce::AudioProcessorValueTreeState &apvts,
                                   const juce::String &parameterID,
                                   const juce::String &unitSuffix)
    : suffix(unitSuffix) {

    // Set up the slider
    rotarySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    rotarySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(rotarySlider);

    // Set up the parameter attachment
    sliderAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, parameterID, rotarySlider);

    // Set up the top label (parameter name)
    addAndMakeVisible(topLabel);
    topLabel.setName("Text");
    topLabel.setText(apvts.getParameter(parameterID)->name,
                     juce::dontSendNotification);
    topLabel.setJustificationType(juce::Justification::centred);
    topLabel.setColour(juce::Label::textColourId, juce::Colour(TEXT_COLOUR));

    // Set up the bottom label (value with units)
    addAndMakeVisible(bottomLabel);
    bottomLabel.setName("Text");
    bottomLabel.setJustificationType(juce::Justification::centred);
    bottomLabel.setColour(juce::Label::textColourId, juce::Colour(TEXT_COLOUR));

    // Update the labels when the slider value changes
    rotarySlider.onValueChange = [this]() { updateLabels(); };

    // Initialize labels
    updateLabels();
}

MyRotatorySlider::~MyRotatorySlider() {}

void MyRotatorySlider::resized() {
    auto bounds = getLocalBounds();
    // Layout the labels and slider
    // The top label is the 10% of the height of the component and has the same
    // width as the component, the bottom label is the same size as the top
    // label and is placed at the bottom of the component. The rotary slider is
    // placed in the center of the component and is 80% of the height of the
    // component.
    //

    topLabel.setBounds(bounds.removeFromTop(static_cast<int>(10)));
    bottomLabel.setBounds(bounds.removeFromBottom(static_cast<int>(10)));
    rotarySlider.setBounds(bounds);
}

void MyRotatorySlider::updateLabels() {

    // Update the value label with units
    // If the suffix is db and the value is -60.0, display "-inf"
    auto value = rotarySlider.getValue();

    if (suffix == "dB" && value <= -60.0)
        bottomLabel.setText("-inf", juce::dontSendNotification);
    else
        bottomLabel.setText(juce::String(value, 2) + " " + suffix,
                            juce::dontSendNotification);
}
