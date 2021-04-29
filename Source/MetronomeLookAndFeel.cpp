/*
  ==============================================================================

    MetronomeLookAndFeel.cpp
    Created: 28 Apr 2021 3:46:56pm
    Author:  Brian Arne

  ==============================================================================
*/

#include "MetronomeLookAndFeel.h"

MetronomeLookAndFeel::MetronomeLookAndFeel()
{
    //TODO: Do this for Fonts, backgrounds, textTyping backgrounds, borders
    //setColour(<#int colourId#>, <#Colour colour#>);
    
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(juce::Colours::limegreen));
    setColour(juce::Slider::trackColourId, juce::Colour(juce::Colours::limegreen));
}

//MetronomeLookAndFeel::getLabelFont(<#Label &#>)
