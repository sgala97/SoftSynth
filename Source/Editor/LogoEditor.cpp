#include "Colours.h"
#include <JuceHeader.h>

class LogoEditor : public juce::Component {
public:
    LogoEditor() {
        addAndMakeVisible(firstWord);
        firstWord.setText("Soft", juce::dontSendNotification);
        firstWord.setJustificationType(juce::Justification::centred);
        firstWord.setColour(juce::Label::textColourId,
                            juce::Colour(TEXT_COLOUR));
        firstWord.setName("Title");

        addAndMakeVisible(secondWord);
        secondWord.setText("Synth", juce::dontSendNotification);
        secondWord.setJustificationType(juce::Justification::centred);
        secondWord.setColour(juce::Label::textColourId,
                             juce::Colour(TEXT_COLOUR));
        secondWord.setName("Title");
    }

    ~LogoEditor() {}

    void paint(juce::Graphics &g) override {
        auto bounds = getLocalBounds();
        bounds = bounds.reduced(15);
        g.setColour(juce::Colour(TEXT_COLOUR));
        g.drawRoundedRectangle(bounds.toFloat(), 20, 3);
    }

    void resized() override {
        auto bounds = getLocalBounds();
        bounds = bounds.reduced(30);
        auto firstWordBounds = bounds.removeFromTop(bounds.getHeight() / 2);
        firstWordBounds.removeFromRight(50);
        auto secondWordBounds = bounds;
        secondWordBounds.removeFromLeft(50);
        firstWord.setBounds(firstWordBounds);
        secondWord.setBounds(secondWordBounds);
    }

private:
    Label firstWord, secondWord;
};
