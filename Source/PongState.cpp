/*
  ==============================================================================

    PongState.cpp
    Created: 13 Apr 2021 7:09:00pm
    Author:  Brian Arne

  ==============================================================================
*/

#include "PongState.h"

PongState::PongState() : m_isPlaying(false), m_previousBar(0), m_isDownBeat(true), m_isReversed(false)
{
    m_leadingEdge = 1;
    
}

PongState::~PongState()
{
    
}
