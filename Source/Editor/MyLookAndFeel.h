#pragma once
#include <JuceHeader.h>

class MyLookAndFeel : public juce::LookAndFeel_V4 {
public:
    MyLookAndFeel();
    ~MyLookAndFeel();

    // Override the drawRotarySlider method to draw a custom rotary slider
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width,
                          int height, float sliderPosProportional,
                          float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider &slider) override;

    // Override the drawToggleButton method to draw a custom toggle button
    void drawToggleButton(juce::Graphics &g, juce::ToggleButton &button,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override;

    // Override createComboBoxTextBox to create a custom text box for the combo box,
    // and setInterceptsMouseClicks to false so that the combo box can receive mouse
    Label *createComboBoxTextBox(ComboBox &box) override;

    // Override the drawComboBox method to draw a custom combo box
    void drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox &box) override;
    
    // Override the drawPopupMenuBackground method to draw a custom popup menu
    void drawPopupMenuBackground(juce::Graphics &g, int width, int height) override;

    // Override the drawPopupMenuItem method to draw a custom popup menu item
    void drawPopupMenuItem(juce::Graphics &g, const juce::Rectangle<int> &area,
                           bool isSeparator, bool isActive, bool isHighlighted,
                           bool isTicked, bool hasSubMenu, const juce::String &text,
                           const juce::String &shortcutKeyText,
                           const juce::Drawable *icon, const juce::Colour *textColour) override;

    // Override getLabelFont to return a custom font for the labels depending on
    // the label's name
    juce::Font getLabelFont(juce::Label &label) override;

    // Override the getComboBoxFont to return a custom font for the combo box
    juce::Font getComboBoxFont(juce::ComboBox &box) override;

private:
    juce::Typeface::Ptr customFont1;
    juce::Typeface::Ptr customFont2;
};
