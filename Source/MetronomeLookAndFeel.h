/*
  ==============================================================================

    MetronomeLookAndFeel.h
    Created: 28 Apr 2021 3:46:56pm
    Author:  Brian Arne

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================
class MetronomeLookAndFeel : public juce::LookAndFeel_V4
{
public:
    //==============================================================================
    
    /**
     Constructor for Metronome look and feel used to set 'hacker' style coloring to the application
     */
    MetronomeLookAndFeel();
};
