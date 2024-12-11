#include "MyComboBox.h"
#include "MyRotatorySlider.h"
#include "MyRoundedBorder.h"
#include "MyToggleButton.h"
#include <JuceHeader.h>

class FilterEditor : public juce::Component {

public:
    FilterEditor(juce::AudioProcessorValueTreeState &apvts)
        : buttonFilter(apvts, "FilterPlaying"),
          sliderFrequency(apvts, "FilterFrequency", "Hz"),
          sliderQ(apvts, "FilterQ", ""),
          comboBoxFilterType(apvts, "FilterType"),
          border(3, 20, "BiQuad Filter") {
        addAndMakeVisible(border);
        addAndMakeVisible(buttonFilter);
        addAndMakeVisible(sliderFrequency);
        addAndMakeVisible(sliderQ);
        addAndMakeVisible(comboBoxFilterType);

        bool enabled = static_cast<juce::AudioParameterBool *>(
                           apvts.getParameter("FilterPlaying"))
                           ->get();
        border.setEnabled(enabled);
        sliderFrequency.setEnabled(enabled);
        sliderQ.setEnabled(enabled);
        comboBoxFilterType.setEnabled(enabled);

        buttonFilter.addCallbackOnValueChange([this](bool enabled) {
            border.setEnabled(enabled);
            border.repaint();
            sliderFrequency.setEnabled(enabled);
            sliderQ.setEnabled(enabled);
            comboBoxFilterType.setEnabled(enabled);
        });
    }

    ~FilterEditor(){};

    void resized() override {
        auto bounds = getLocalBounds();
        border.setBounds(bounds);
        bounds = bounds.reduced(30);
        buttonFilter.setBounds(
            bounds.removeFromLeft(75).withSizeKeepingCentre(75, 38));

        auto comboBoxBounds = bounds.removeFromRight(80);
        comboBoxBounds.removeFromBottom(25);
        comboBoxFilterType.setBounds(
            comboBoxBounds.withSizeKeepingCentre(80, 50));

        bounds.reduce(10, 0);
        float sliderWidth = bounds.toFloat().getWidth() / 2;
        sliderFrequency.setBounds(
            bounds.removeFromLeft(sliderWidth).withSizeKeepingCentre(70, 90));
        sliderQ.setBounds(
            bounds.removeFromLeft(sliderWidth).withSizeKeepingCentre(70, 90));
    };

private:
    MyToggleButton buttonFilter;
    MyRotatorySlider sliderFrequency, sliderQ;
    MyComboBox comboBoxFilterType;

    MyRoundedBorder border;
};
