#include "MyRoundedBorder.h"
#include "juce_graphics/juce_graphics.h"
MyRoundedBorder::MyRoundedBorder(float borderThickness, float cornerRadius,
                                 juce::String headingText = "")
    : borderThickness(borderThickness), cornerRadius(cornerRadius) {

    if (headingText.isNotEmpty()) {
        addAndMakeVisible(headingLabel);
        headingLabel.setName("Heading");
        headingLabel.setText(headingText, juce::dontSendNotification);
        headingLabel.setJustificationType(juce::Justification::centred);
        headingLabel.setColour(juce::Label::textColourId,
                               juce::Colour(TEXT_COLOUR));
    } else {
        headingLabel.setText("", juce::dontSendNotification);
    }

    customFont = juce::Typeface::createSystemTypefaceFor(
        BinaryData::Normal_otf, BinaryData::Normal_otfSize);
}

MyRoundedBorder::~MyRoundedBorder() {}

void MyRoundedBorder::paint(juce::Graphics &g) {
    // Set the color for the border
    auto colour = isEnabled() ? juce::Colour(TEXT_COLOUR)
                              : juce::Colour(TEXT_COLOUR_DISABLED);
    g.setColour(colour);

    // Define the bounds of the rectangle (with some padding in top and left)
    auto bounds = getLocalBounds().toFloat().reduced(15);

    // Draw the rectangle with rounded corners
    g.drawRoundedRectangle(bounds, cornerRadius, borderThickness);

    if (headingLabel.getText().isNotEmpty()) {

        // Set the color for the text

        auto bounds = getLocalBounds();
        bounds.removeFromLeft(15+30);
        bounds.removeFromTop(5);

        juce::Rectangle<int> textBounds = calculateTextDimensionsWithCustomFont(
            headingLabel.getText(), 20.0f);

        auto backgroundBounds = juce::Rectangle<int>(
            bounds.getX(), bounds.getY(), textBounds.getWidth() + 8,
            textBounds.getHeight());
        

        // Draw the background
        g.setColour(juce::Colour(BACKGROUND_COLOUR));
        g.fillRect(backgroundBounds.toFloat());

        textBounds = backgroundBounds.withSizeKeepingCentre(
            textBounds.getWidth(), textBounds.getHeight());

        // Draw the text
        headingLabel.setBounds(textBounds);
    }
}

juce::Rectangle<int>
MyRoundedBorder::calculateTextDimensionsWithCustomFont(const juce::String &text,
                                                       float customHeight) {
    // Load the custom font from a file

    // Create a Font object with the custom typeface and desired height
    juce::Font font{juce::FontOptions(customFont)};
    font.setHeight(customHeight);

    // Calculate the width of the text in pixels
    int textWidth = font.getStringWidth(text);

    // Get the height of the font (which is the height of the text)
    int textHeight = static_cast<int>(font.getHeight());

    // Return the dimensions of the text
    return juce::Rectangle<int>(0, 0, textWidth, textHeight);
}
