/*
  ==============================================================================

    MetronomeLookAndFeel.cpp
    Created: 28 Apr 2021 3:46:56pm
    Author:  Brian Arne

  ==============================================================================
*/

#include "MetronomeLookAndFeel.h"

//==============================================================================
MetronomeLookAndFeel::MetronomeLookAndFeel()
{
    const juce::Colour hackerColours = juce::Colours::limegreen;
    
    // Slider
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0));
    setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0));
    setColour(juce::Slider::textBoxHighlightColourId, juce::Colour(0));
    setColour(juce::Slider::trackColourId, juce::Colour(juce::Colours::limegreen));
    setColour(juce::Slider::trackColourId, hackerColours);

    // Label
    setColour(juce::Label::ColourIds::textColourId, hackerColours);
    
    // Button
    setColour(juce::DrawableButton::ColourIds::textColourId, hackerColours);
    setColour(juce::TextButton::ColourIds::textColourOnId, hackerColours);
    setColour(juce::TextButton::ColourIds::textColourOffId, hackerColours);
    setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(juce::Colours::black));
    setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(juce::Colours::darkgrey));
    setColour(juce::ComboBox::ColourIds::outlineColourId, hackerColours);

}
