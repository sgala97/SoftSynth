#pragma once
#include "Colours.h"
#include <JuceHeader.h>
class MyRoundedBorder : public juce::Component {
public:
    MyRoundedBorder(float borderThickness, float cornerRadius,
                    juce::String headingText);
    ~MyRoundedBorder();

    void paint(juce::Graphics &g) override;

private:
    float borderThickness;
    float cornerRadius;
    juce::Label headingLabel;
    juce::Typeface::Ptr customFont;

    juce::Rectangle<int>
    calculateTextDimensionsWithCustomFont(const juce::String &text,
                                          float customHeight);
};
