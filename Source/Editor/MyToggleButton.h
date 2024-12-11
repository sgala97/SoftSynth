#pragma once
#include <JuceHeader.h>

class MyToggleButton : public juce::Component {
public:
    MyToggleButton(juce::AudioProcessorValueTreeState &apvts,
                   const juce::String &parameterID);
    ~MyToggleButton();
    void resized() override;

    void addCallbackOnValueChange(std::function<void(bool)> callback);

    bool getToggleState();

private:
    juce::ToggleButton button;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
        buttonAttachment;
};
