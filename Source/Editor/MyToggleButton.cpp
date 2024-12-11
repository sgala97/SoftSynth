#include "MyToggleButton.h"

MyToggleButton::MyToggleButton(juce::AudioProcessorValueTreeState &apvts,
                               const juce::String &parameterID) {
    // no text on the button
    button.setButtonText("");
    addAndMakeVisible(button);

    buttonAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts, parameterID, button);
}

MyToggleButton::~MyToggleButton() {}

void MyToggleButton::resized() { button.setBounds(getLocalBounds()); }

void MyToggleButton::addCallbackOnValueChange(std::function<void(bool)> callback) {
    button.onStateChange = [callback, this]() {
        callback(button.getToggleState());
    };
}

bool MyToggleButton::getToggleState() { return button.getToggleState(); }
