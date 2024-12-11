#include "MyLookAndFeel.h"
#include "Colours.h"
#include "juce_graphics/juce_graphics.h"

MyLookAndFeel::MyLookAndFeel() {
    customFont1 = juce::Typeface::createSystemTypefaceFor(
        BinaryData::Normal_otf, BinaryData::Normal_otfSize);
    customFont2 = juce::Typeface::createSystemTypefaceFor(
        BinaryData::Italic_otf, BinaryData::Italic_otfSize);
}
MyLookAndFeel::~MyLookAndFeel() {}

// Override the drawRotarySlider method to draw a custom rotary slider
// with black outside circle and white inside circle with a black rectangle from
// the center to the slider position
void MyLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width,
                                     int height, float sliderPos,
                                     float rotaryStartAngle,
                                     float rotaryEndAngle,
                                     juce::Slider &slider) {

    // Set the color for the slider
    auto colour = slider.isEnabled() ? juce::Colour(TEXT_COLOUR)
                                     : juce::Colour(TEXT_COLOUR_DISABLED);

    // Define a margin for the outside curve to fit within the defined bounds
    const float margin = 6.0f;
    const float arcThickness = 3.0f;

    // Calculate the maximum radius that fits within the given width and height
    auto maxRadius =
        (float)juce::jmin(width, height) / 2.0f - margin - arcThickness;

    // Reduce the radius for the main slider components (ellipse and pointer)
    auto radius = maxRadius - margin;

    // Calculate the center of the slider
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;

    // Calculate the position and size of the ellipse
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;

    // Calculate the angle for the pointer
    auto angle =
        rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Set the color for the background depending on the mouse state
    if (slider.isEnabled()) {
        auto backgroundColour = slider.isMouseOver()
                                    ? juce::Colour(TEXT_COLOUR_HIGHLIGHTED)
                                    : juce::Colour(BACKGROUND_COLOUR);

        backgroundColour = slider.isMouseButtonDown()
                               ? juce::Colour(TEXT_COLOUR_MOUSE_PRESSED)
                               : backgroundColour;

        // Draw the fill of the ellipse
        g.setColour(juce::Colour(backgroundColour));
        // Draw the ellipse with a small margin
        g.fillEllipse(rx + 3, ry + 3, rw - 6, rw - 6);
        // Draw the pointer with a bigger margin with background color for
        // create a gap between the hightled or pressed color with the pointer
        juce::Path p;
        auto pointerLength = radius * 0.50f + 3.0f;
        auto pointerThickness = 6.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness,
                       pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(
            centreX, centreY));

        g.setColour(juce::Colour(BACKGROUND_COLOUR));
        g.fillPath(p);
    }

    // Draw the outline of the ellipse
    g.setColour(juce::Colour(colour));
    g.drawEllipse(rx, ry, rw, rw, 3.0f);

    // Draw the outside curve (arc)
    juce::Path arcPath;
    arcPath.addCentredArc(centreX, centreY, maxRadius, maxRadius, 0.0f,
                          rotaryStartAngle, angle, true);

    // Set the arc's color and thickness
    g.setColour(juce::Colour(colour));
    g.strokePath(arcPath, juce::PathStrokeType(arcThickness));

    // Draw the pointer
    juce::Path p;
    auto pointerLength = radius * 0.5f;
    auto pointerThickness = 3.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness,
                   pointerLength);
    p.applyTransform(
        juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    g.setColour(juce::Colour(colour));
    g.fillPath(p);
}

void MyLookAndFeel::drawToggleButton(juce::Graphics &g,
                                     juce::ToggleButton &button,
                                     bool shouldDrawButtonAsHighlighted,
                                     bool shouldDrawButtonAsDown) {
    float cornerRadius = 10.0f;
    float borderThickness = 3.0f;
    auto bounds =
        button.getLocalBounds().toFloat().reduced(borderThickness / 2);

    auto colour = button.isEnabled() ? juce::Colour(TEXT_COLOUR)
                                     : juce::Colour(TEXT_COLOUR_DISABLED);

    // Draw button rounded border
    g.setColour(colour);
    g.drawRoundedRectangle(bounds, cornerRadius, borderThickness);

    // Get button on indicator bounds
    float removeFromTopAndBottom = bounds.getHeight() * 0.25;
    float removeFromLeftAndRight = bounds.getWidth() * 0.2;
    auto indicatorBounds = bounds.toFloat().reduced(removeFromLeftAndRight,
                                                    removeFromTopAndBottom);

    if (button.isEnabled()) {
        // Draw background in button for highlighted and down states
        auto backgroundColour = shouldDrawButtonAsHighlighted
                                    ? juce::Colour(TEXT_COLOUR_HIGHLIGHTED)
                                    : juce::Colour(BACKGROUND_COLOUR);

        backgroundColour = shouldDrawButtonAsDown
                               ? juce::Colour(TEXT_COLOUR_MOUSE_PRESSED)
                               : backgroundColour;

        g.setColour(backgroundColour);
        g.fillRoundedRectangle(bounds.reduced(borderThickness / 2 + 3),
                               cornerRadius - borderThickness / 2);

        // Draw background in indicator for show a margin between the indicator
        // and the highlight or down color when the button is enabled
        if (button.getToggleState()) {
            g.setColour(juce::Colour(BACKGROUND_COLOUR));
            g.fillRoundedRectangle(indicatorBounds,
                                   cornerRadius - borderThickness / 2);
        }
    }

    g.setColour(colour);
    // Draw button rounded rectangle indicator in case the button enabled
    if (button.getToggleState())
        g.fillRoundedRectangle(indicatorBounds.reduced(3),
                               cornerRadius - borderThickness / 2);
}

Label *MyLookAndFeel::createComboBoxTextBox(juce::ComboBox &box) {
    Label *label = new Label(String(), String());
    label->setInterceptsMouseClicks(false, false);
    return label;
}

void MyLookAndFeel::drawComboBox(juce::Graphics &g, int width, int height,
                                 bool isButtonDown, int, int, int, int,
                                 juce::ComboBox &box) {
    auto cornerRadius = 5.0f;
    auto borderThickness = 3.0f;

    auto bounds = box.getLocalBounds().toFloat().reduced(borderThickness / 2);

    // Set the color for the border

    auto colour = box.isEnabled() ? juce::Colour(TEXT_COLOUR)
                                  : juce::Colour(TEXT_COLOUR_DISABLED);

    g.setColour(juce::Colour(colour));
    g.drawRoundedRectangle(bounds, cornerRadius, borderThickness);

    if (box.isEnabled()) {
        auto backgroundColour = box.isMouseOver()
                                    ? juce::Colour(TEXT_COLOUR_HIGHLIGHTED)
                                    : juce::Colour(BACKGROUND_COLOUR);
        backgroundColour = isButtonDown
                               ? juce::Colour(TEXT_COLOUR_MOUSE_PRESSED)
                               : backgroundColour;

        g.setColour(backgroundColour);
        g.fillRoundedRectangle(bounds.reduced(borderThickness / 2 + 3),
                               cornerRadius - borderThickness / 2);
    }

    // Draw and arrow in the right side of the combo box and rotate it 90 by the
    // center of the arrow if the popup is active
    juce::Path path;
    auto arrowX = static_cast<float>(width) - 20.0f;
    auto arrowY = static_cast<float>(height) * 0.5f;
    path.addTriangle(arrowX, arrowY - 4.0f, arrowX + 8.0f, arrowY, arrowX,
                     arrowY + 4.0f);

    // Rotate the arrow 90 degrees if the popup is active by the center of the
    // arrow
    if (box.isPopupActive())
        path.applyTransform(juce::AffineTransform::rotation(
            juce::MathConstants<float>::halfPi, arrowX, arrowY));

    g.setColour(colour);
    g.fillPath(path);
}

void MyLookAndFeel::drawPopupMenuBackground(juce::Graphics &g, int width,
                                            int height) {
    auto cornerRadius = 10.0f;

    // Draw the background for the popup menu

    g.setColour(juce::Colour(BACKGROUND_COLOUR));
    g.fillRoundedRectangle(1.0f, 0.0f, (float)width - 5, (float)height,
                           cornerRadius);

    // Draw a border around the popup menu
    g.setColour(juce::Colour(TEXT_COLOUR));
    g.drawRoundedRectangle(1.0f, 0.0f, (float)width - 5, (float)height,
                           cornerRadius, 3.0f);
}

void MyLookAndFeel::drawPopupMenuItem(
    juce::Graphics &g, const juce::Rectangle<int> &area, bool isSeparator,
    bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
    const juce::String &text, const juce::String &shortcutKeyText,
    const juce::Drawable *icon, const juce::Colour *textColour) {
    if (isSeparator) {
        g.setColour(juce::Colour(TEXT_COLOUR));
        g.fillRect(area.reduced(5, 0).withHeight(1));
        return;
    }

    auto textColourToUse = juce::Colour(TEXT_COLOUR);

    auto backgroundColour = isHighlighted
                                ? juce::Colour(TEXT_COLOUR_HIGHLIGHTED)
                                : juce::Colour(BACKGROUND_COLOUR);

    // Draw the background for the item
    g.setColour(backgroundColour);
    auto backgroundArea = area.toFloat().reduced(2);
    backgroundArea.removeFromLeft(2);
    backgroundArea.removeFromRight(5);
    g.fillRoundedRectangle(backgroundArea, 10.0f - 4.5f);


    // Draw the text
    g.setColour(textColourToUse);
    g.setFont(juce::FontOptions(customFont1).withHeight(16.0f));
    g.drawFittedText(text, area.reduced(6, 0),
                     juce::Justification::centredLeft, 1);

    if (isTicked) {
        auto tickWidth = area.getHeight() * 0.7f;
        auto tickArea =
            area.withWidth(tickWidth).withX(area.getRight() - tickWidth);

        g.setColour(juce::Colour(TEXT_COLOUR));
        g.fillRoundedRectangle(tickArea.toFloat().reduced(4),2.0f);
    }
}

juce::Font MyLookAndFeel::getLabelFont(juce::Label &label) {
    if (label.getName() == "Text") {
        return juce::Font(juce::FontOptions(customFont1)).withHeight(16.0f);
    } else if (label.getName() == "Heading") {
        return juce::Font(juce::FontOptions(customFont1)).withHeight(20.0f);
    } else if (label.getName() == "Title") {
        return juce::Font(juce::FontOptions(customFont2)).withHeight(70.0f);
    } else {
        return juce::LookAndFeel_V4::getLabelFont(label).withHeight(16.0f);
    }
}

juce::Font MyLookAndFeel::getComboBoxFont(juce::ComboBox &box) {
    return juce::Font(juce::FontOptions(customFont1)).withHeight(16.0f);
}
