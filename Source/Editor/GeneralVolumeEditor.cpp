#include "MyRotatorySlider.h"
#include "MyRoundedBorder.h"
#include <JuceHeader.h>

class GeneralVolumeEditor : public juce::Component {
public:
    GeneralVolumeEditor(juce::AudioProcessorValueTreeState &apvts)
        : sliderVolume(apvts, "Volume", "dB"), border(3, 20, "Main Volume") {
        addAndMakeVisible(border);
        addAndMakeVisible(sliderVolume);
    }
    ~GeneralVolumeEditor(){};
    void resized() override {
        auto bounds = getLocalBounds();
        border.setBounds(bounds);
        // 200 - 60 = 140
        // 175 - 60 = 115
        sliderVolume.setBounds(bounds.reduced(30).withSizeKeepingCentre(90, 110));
    };

private:
    MyRotatorySlider sliderVolume;
    MyRoundedBorder border;
};
