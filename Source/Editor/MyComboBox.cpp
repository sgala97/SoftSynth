#include "MyComboBox.h"
#include "juce_core/juce_core.h"

MyComboBox::MyComboBox(juce::AudioProcessorValueTreeState &apvts,
                       const juce::String &parameterID) {

    addAndMakeVisible(label);

    label.setText(apvts.getParameter(parameterID)->name,
                  juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centredLeft);
    label.setColour(juce::Label::textColourId, juce::Colour(TEXT_COLOUR));
    label.setName("Text");

    addAndMakeVisible(comboBox);
    comboBox.setColour(ComboBox::textColourId, Colour(TEXT_COLOUR));

    // Get combobox items from the parameter
    juce::StringArray items =
        apvts.getParameter(parameterID)->getAllValueStrings();
    for (int i = 0; i < items.size(); i++) {
        comboBox.addItem(items[i], i + 1);
    }

    comboBoxAttachment = std::make_unique<
        juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, parameterID, comboBox);
}

MyComboBox::~MyComboBox() {}

void MyComboBox::resized() {
    auto bounds = getLocalBounds();
    label.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.5));
    comboBox.setBounds(bounds);
}

void MyComboBox::addCallbackOnValueChange(std::function<void(int)> callback) {
    comboBox.onChange = [callback, this]() {
        callback(comboBox.getSelectedId());
    };
}

int MyComboBox::getSelectedId() { return comboBox.getSelectedId(); }
