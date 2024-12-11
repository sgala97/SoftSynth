#include "MyRotatorySlider.h"
#include "MyRoundedBorder.h"
#include "MyToggleButton.h"
#include <JuceHeader.h>

class DelayEditor : public juce::Component {

public:
    DelayEditor(juce::AudioProcessorValueTreeState &apvts)
        : delayButton(apvts, "DelayPlaying"),
          sliderTime(apvts, "DelayTime", "s"),
          sliderFeedback(apvts, "DelayFeedback", "dB"),
          sliderWet(apvts, "DelayWet", "dB"),
          sliderDry(apvts, "DelayDry", "dB"), border(3, 20, "Delay") {
        addAndMakeVisible(border);
        addAndMakeVisible(delayButton);
        addAndMakeVisible(sliderTime);
        addAndMakeVisible(sliderFeedback);
        addAndMakeVisible(sliderWet);
        addAndMakeVisible(sliderDry);

        bool enabled = static_cast<juce::AudioParameterBool *>(
                           apvts.getParameter("DelayPlaying"))
                           ->get();

        border.setEnabled(enabled);
        sliderTime.setEnabled(enabled);
        sliderFeedback.setEnabled(enabled);
        sliderWet.setEnabled(enabled);
        sliderDry.setEnabled(enabled);

        delayButton.addCallbackOnValueChange([this](bool enabled) {
            border.setEnabled(enabled);
            border.repaint();
            sliderTime.setEnabled(enabled);
            sliderFeedback.setEnabled(enabled);
            sliderWet.setEnabled(enabled);
            sliderDry.setEnabled(enabled);
        });
    };

    ~DelayEditor(){};

    void resized() override {
        auto bounds = getLocalBounds();
        border.setBounds(bounds);
        bounds = bounds.reduced(30);
        delayButton.setBounds(
            bounds.removeFromLeft(75).withSizeKeepingCentre(75, 38));

        bounds.removeFromLeft(10);

        float sliderWidth = bounds.toFloat().getWidth() / 4;
        sliderTime.setBounds(
            bounds.removeFromLeft(sliderWidth).withSizeKeepingCentre(70, 90));
        sliderFeedback.setBounds(
            bounds.removeFromLeft(sliderWidth).withSizeKeepingCentre(70, 90));
        sliderWet.setBounds(
            bounds.removeFromLeft(sliderWidth).withSizeKeepingCentre(70, 90));
        sliderDry.setBounds(
            bounds.removeFromLeft(sliderWidth).withSizeKeepingCentre(70, 90));
    };

private:
    MyToggleButton delayButton;
    MyRotatorySlider sliderTime, sliderFeedback, sliderWet, sliderDry;

    MyRoundedBorder border;
};
