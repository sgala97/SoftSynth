#include "MyComboBox.h"
#include "MyRotatorySlider.h"
#include "MyRoundedBorder.h"
#include "MyToggleButton.h"
#include "juce_core/juce_core.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

class OscillatorEditor : public juce::Component {
public:
    OscillatorEditor(juce::AudioProcessorValueTreeState &apvts,
                     int oscillatorIndex)
        : border(3, 20, "Oscillator " + std::to_string(oscillatorIndex)),
          toggleOscillator(apvts, "WaveTablePlaying" +
                                      std::to_string(oscillatorIndex)),
          sliderVolume(
              apvts, "WaveTableVolume" + std::to_string(oscillatorIndex), "dB"),
          waveFormComboBox(apvts,
                           "WaveTableType" + std::to_string(oscillatorIndex)) {
        addAndMakeVisible(border);
        addAndMakeVisible(toggleOscillator);
        addAndMakeVisible(sliderVolume);
        addAndMakeVisible(waveFormComboBox);
        
        for (size_t i = 1; i <= 12; i++) {
            sliders.emplace_back(std::make_unique<MyRotatorySlider>(
                apvts,
                "Oscillator" + std::to_string(oscillatorIndex) + "A" +
                    std::to_string(i),
                ""));

            addAndMakeVisible(*sliders.back());
        }

        bool enabled = static_cast<juce::AudioParameterBool *>(
                           apvts.getParameter("WaveTablePlaying" +
                                              std::to_string(oscillatorIndex)))
                           ->get();
        bool customWaveForm =
            static_cast<juce::AudioParameterChoice *>(
                apvts.getParameter("WaveTableType" +
                                   std::to_string(oscillatorIndex)))
                ->getIndex() == 0;

        // Check if the oscillator is enabled
        border.setEnabled(enabled);
        sliderVolume.setEnabled(enabled);
        waveFormComboBox.setEnabled(enabled);
        // Check if the waveform is custom and the oscillator is enabled
        for (size_t i = 0; i < 12; i++) {
            sliders[i]->setEnabled(enabled && customWaveForm);
        }
        
        toggleOscillator.addCallbackOnValueChange([this](bool state) {
            bool customWaveForm = waveFormComboBox.getSelectedId() == 1;
            // Check if the oscillator is enabled
            border.setEnabled(state);
            border.repaint();
            sliderVolume.setEnabled(state);
            waveFormComboBox.setEnabled(state);
            // Check if the waveform is custom and the oscillator is enabled
            for (size_t i = 0; i < 12; i++) {
                sliders[i]->setEnabled(state && customWaveForm);
            }
        });
        
        waveFormComboBox.addCallbackOnValueChange([this](int id) {
            for (size_t i = 0; i < 12; i++) {
                sliders[i]->setEnabled(id == 1 &&
                                       toggleOscillator.getToggleState());
            }
        });

    }

    ~OscillatorEditor(){};
    void resized() override {
        auto bounds = getLocalBounds();
        border.setBounds(bounds);
        bounds = bounds.reduced(30);

        toggleOscillator.setBounds(
            bounds.removeFromLeft(75).withSizeKeepingCentre(75, 38));

        bounds.removeFromLeft(10);

        sliderVolume.setBounds(
            bounds.removeFromLeft(70).withSizeKeepingCentre(70, 90));

        bounds.removeFromLeft(10);

        auto comboBoxBounds = bounds.removeFromLeft(80);
        comboBoxBounds.removeFromBottom(25);
        waveFormComboBox.setBounds(
            comboBoxBounds.withSizeKeepingCentre(80, 50));

        bounds.removeFromLeft(30);

        float sliderWidth = bounds.toFloat().getWidth() / 12;

        for (size_t i = 0; i < 12; i++) {
            // Split sliderBound in 12 equal parts
            auto sliderBounds = bounds.removeFromLeft(sliderWidth);
            sliders[i]->setBounds(sliderBounds.withSizeKeepingCentre(
                sliderWidth, sliderWidth + 20));
        }
    };

private:
    std::vector<std::unique_ptr<MyRotatorySlider>> sliders;
    MyRoundedBorder border;
    MyToggleButton toggleOscillator;
    MyRotatorySlider sliderVolume;
    MyComboBox waveFormComboBox;
};
