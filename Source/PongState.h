/*
  ==============================================================================

    PongState.h
    Created: 13 Apr 2021 7:09:00pm
    Author:  Brian Arne

  ==============================================================================
*/

#pragma once

struct PongState{
    PongState();
    ~PongState();
   
    bool m_isPlaying;
    bool m_isDownBeat;
    bool m_isReversed;
    
    int m_leadingEdge;
    
    // TODO: Can remove later in favor or leadingEdge impl.
    int m_previousBar;
};
