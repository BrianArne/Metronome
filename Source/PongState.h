/*
  ==============================================================================

    PongState.h
    Created: 13 Apr 2021 7:09:00pm
    Author:  Brian Arne

  ==============================================================================
*/

#pragma once

/**
 A class to maintain the state of a metronome
 
 m_isReversed is used to indicate the corresponding PongComponent MovingGradient is moving.
 m_isReversed == false indicates left to right and vice versa
 
 @see PongComponent, PongComponent::MovingGradient
 */
//==============================================================================
struct PongState{
    //==============================================================================

    /**
     Constructor for PongState. Defaults m_isPlaying to false, m_isDownBeat to true and m_isReversed to false
     
     Created in a state under the assumption that the metronome is not playing and will begin on a down beat
     */
    PongState();
    
    /**
     Defaulted deconstructor for PongState
     */
    ~PongState() = default;
    
    //==============================================================================
    bool m_isPlaying;
    bool m_isDownBeat;
    bool m_isReversed;
};

