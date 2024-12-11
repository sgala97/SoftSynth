#include "MyRoundedBorder.h"
#include "MyToggleButton.h"
#include <JuceHeader.h>

class NoiseEditor : public juce::Component {
public:
    NoiseEditor(juce::AudioProcessorValueTreeState &apvts,
                const std::vector<Component *> &componentsToDisable)
        : noiseButton(apvts, "Noise"), border(3, 20, "White Noise"),
          componentsToDisable(componentsToDisable) {
        addAndMakeVisible(border);
        addAndMakeVisible(noiseButton);


        bool enabled = static_cast<juce::AudioParameterBool *>(
                           apvts.getParameter("Noise"))
                           ->get();
        
        for (auto *component : this->componentsToDisable) {
            if (component != nullptr) {
                component->setEnabled(!enabled);
                component->repaint();
            } else
                std::cerr << "Component is nullptr";
        }

        noiseButton.addCallbackOnValueChange([this](bool enabled) {
            for (auto *component : this->componentsToDisable) {
                if (component != nullptr) {
                    component->setEnabled(!enabled);
                    component->repaint();
                } else
                    std::cerr << "Component is nullptr";
            }
        });
    }
    ~NoiseEditor(){};
    void resized() override {
        auto bounds = getLocalBounds();
        border.setBounds(bounds);
        noiseButton.setBounds(bounds.reduced(30).withSizeKeepingCentre(75, 38));
    };

private:
    MyToggleButton noiseButton;
    MyRoundedBorder border;

    // Reference for async call
    std::vector<Component *> componentsToDisable;
};
