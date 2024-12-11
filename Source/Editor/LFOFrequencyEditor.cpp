#include "MyComboBox.h"
#include "MyRotatorySlider.h"
#include "MyRoundedBorder.h"
#include "MyToggleButton.h"
#include <JuceHeader.h>

class LFOFrequencyEditor : public juce::Component {

public:
    LFOFrequencyEditor(juce::AudioProcessorValueTreeState &apvts)
        : buttonLFO(apvts, "LFOFPlaying"),
          sliderFrequency(apvts, "FrequencyLFOF", "Hz"),
          sliderAmount(apvts, "CentsLFOF", ""),
          comboBoxWaveform(apvts, "WaveTableTypeLFOF"),
          border(3, 20, "LFO Frequency") {
        addAndMakeVisible(border);
        addAndMakeVisible(buttonLFO);
        addAndMakeVisible(sliderFrequency);
        addAndMakeVisible(sliderAmount);
        addAndMakeVisible(comboBoxWaveform);

        bool enabled = static_cast<juce::AudioParameterBool *>(
                           apvts.getParameter("LFOFPlaying"))
                           ->get();
        border.setEnabled(enabled);
        sliderFrequency.setEnabled(enabled);
        sliderAmount.setEnabled(enabled);
        comboBoxWaveform.setEnabled(enabled);

        buttonLFO.addCallbackOnValueChange([this](bool enabled) {
            border.setEnabled(enabled);
            border.repaint();
            sliderFrequency.setEnabled(enabled);
            sliderAmount.setEnabled(enabled);
            comboBoxWaveform.setEnabled(enabled);
        });
    }

    ~LFOFrequencyEditor(){};

    void resized() override {
        auto bounds = getLocalBounds();
        border.setBounds(bounds);
        bounds = bounds.reduced(30);
        buttonLFO.setBounds(
            bounds.removeFromLeft(75).withSizeKeepingCentre(75, 38));

        auto comboBoxBounds = bounds.removeFromRight(80);
        comboBoxBounds.removeFromBottom(25);
        comboBoxWaveform.setBounds(
            comboBoxBounds.withSizeKeepingCentre(80, 50));

        bounds.reduce(10,0);
        float sliderWidth = bounds.toFloat().getWidth() / 2;
        sliderFrequency.setBounds(
            bounds.removeFromLeft(sliderWidth).withSizeKeepingCentre(70, 90));
        sliderAmount.setBounds(
            bounds.removeFromLeft(sliderWidth).withSizeKeepingCentre(70, 90));
    };

private:
    MyToggleButton buttonLFO;
    MyRotatorySlider sliderFrequency, sliderAmount;
    MyComboBox comboBoxWaveform;

    MyRoundedBorder border;
};
