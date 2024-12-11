#pragma once
#include <JuceHeader.h>
#include "Colours.h"

class MyComboBox : public juce::Component {
public:
    MyComboBox(juce::AudioProcessorValueTreeState &apvts,
               const juce::String &parameterID);
    ~MyComboBox();
    void resized() override;

    void addCallbackOnValueChange(std::function<void(int)> callback);

    int getSelectedId();



private:
    juce::ComboBox comboBox;
    juce::Label label;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        comboBoxAttachment;
};
