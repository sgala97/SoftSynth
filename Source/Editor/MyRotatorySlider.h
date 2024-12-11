#pragma once
#include <JuceHeader.h>

class MyRotatorySlider : public juce::Component {
public:
    MyRotatorySlider(juce::AudioProcessorValueTreeState &apvts,
                     const juce::String &parameterID,
                     const juce::String &unitSuffix);

    ~MyRotatorySlider();

    void resized() override;

private:
    void updateLabels();

    juce::Slider rotarySlider;
    juce::Label topLabel, bottomLabel;
    juce::String suffix;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        sliderAttachment;
};
