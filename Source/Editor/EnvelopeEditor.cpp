#include "MyRotatorySlider.h"
#include "MyRoundedBorder.h"
#include <JuceHeader.h>
class EnvelopeEditor : public juce::Component {
public:
    EnvelopeEditor(juce::AudioProcessorValueTreeState &apvts)
        : sliderAttack(apvts, "AttackA", "s"),
          sliderDecay(apvts, "DecayA", "s"),
          sliderSustain(apvts, "SustainA", ""),
          sliderRelease(apvts, "ReleaseA", "s"),
          border(3, 20, "Envelope Amplitude") {
        addAndMakeVisible(border);
        addAndMakeVisible(sliderAttack);
        addAndMakeVisible(sliderDecay);
        addAndMakeVisible(sliderSustain);
        addAndMakeVisible(sliderRelease);
    }
    ~EnvelopeEditor(){};
    void resized() override {
        auto bounds = getLocalBounds();
        border.setBounds(bounds);
        bounds = bounds.reduced(30);
        // 200 - 60 = 140
        // 175 - 60 = 115
        float sliderWidth = bounds.toFloat().getWidth() / 4;
        sliderAttack.setBounds(
            bounds.removeFromLeft(sliderWidth).withSizeKeepingCentre(70, 90));
        sliderDecay.setBounds(
            bounds.removeFromLeft(sliderWidth).withSizeKeepingCentre(70, 90));
        sliderSustain.setBounds(
            bounds.removeFromLeft(sliderWidth).withSizeKeepingCentre(70, 90));
        sliderRelease.setBounds(
            bounds.removeFromLeft(sliderWidth).withSizeKeepingCentre(70, 90));
    };

private:
    MyRotatorySlider sliderAttack, sliderDecay, sliderSustain, sliderRelease;
    MyRoundedBorder border;
};
