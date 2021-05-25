/*
  ==============================================================================

    PongComponent.cpp
    Created: 10 Apr 2021 5:40:16pm
    Author:  Brian Arne

  ==============================================================================
*/

#include "PongComponent.h"

PongComponent::PongComponent() : m_newState(State::STOPPED), m_isReversed(false)
{
    setFramesPerSecond(100);
}

void PongComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    // Rectangle is drawn whether playing or not. ChangeState() will set the rectangle to 0,0,0,0 if not playing
    auto lastRec = m_gradient.getRectangle();
    if(lastRec.getRight() == getLocalBounds().getRight()){
        m_isReversed = true;
    }else if(lastRec.getX() == 0){
        m_isReversed = false;
    }

    m_gradient.updateRectangle(lastRec.getX(), lastRec.getY(), 10, lastRec.getHeight(), m_isReversed);
    g.setGradientFill(m_gradient.getColourGradient());
    g.fillRect(m_gradient.getRectangle());
}

void PongComponent::update()
{
    
    // Called at interval that is set by setFramesPerSecond()
    //TODO: Within this method we could check to see how much the rectangle/component should be moved
    /*
    function() that returns how many pixels are covered per mIlliseconds passed
    getMillisecondsSinceLastUpdate();
     */

}

void PongComponent::initializeGradientArea()
{
    auto localBounds = getLocalBounds();
    m_gradient.setRectangle(juce::Rectangle<int>(localBounds.getX(), localBounds.getY(), 5, localBounds.getHeight()), m_isReversed);
}

void PongComponent::changeState(State state)
{
    if ( m_newState != state){ // m_newState is a poor naming choice, we should swap state and new state naming
        m_newState = state;
        switch(m_newState)
        {
            case STOPPED:
                m_gradient.updateRectangle(0, 0, 0, 0, false);
                break;
            case STARTING:
                initializeGradientArea();
                changeState(PLAYING);
                break;
            case PLAYING:
                break;
            case STOPPING:
                changeState(STOPPED);
                break;
        }
    }
}

PongComponent::State PongComponent::getState()
{
    return m_newState;
}

PongComponent::MovingGradient::MovingGradient(){}

void PongComponent::MovingGradient::reduceGradientSize()
{
    //TODO: Implement or remove, do we want to reduce the size
    //      once the bar has hit the end? or continue to bounce
    //      it back, not reduce
}

void PongComponent::MovingGradient::increaseGradientSize()
{
    //TODO: Implement or remove, do we want to increase the size
    //      once the bar has bounced back?
}

juce::Rectangle<int> PongComponent::MovingGradient::getRectangle()
{
    return m_rectangle;
}

void PongComponent::MovingGradient::updateRectangle(int newX, int newY, int newWidth, int newHeight, bool isReversed)
{
    int updateVal = isReversed ? -10 : 10;
    if (newWidth != 0){
        m_rectangle.setBounds(newX+updateVal, newY, newWidth, newHeight);
    }else{
        m_rectangle.setBounds(newX, newY, newWidth, newHeight);
    }
    setColourGradient(isReversed);
}

void PongComponent::MovingGradient::setRectangle(juce::Rectangle<int> rec, bool isReversed)
{
    m_rectangle = rec;
    setColourGradient(isReversed);
}

juce::ColourGradient PongComponent::MovingGradient::getColourGradient()
{
    return m_colourGradient;
}

void PongComponent::MovingGradient::setColourGradient(const bool isReversed)
{
    if(isReversed){
        m_colourGradient = juce::ColourGradient::horizontal(juce::Colours::black, m_rectangle.getRight(),
                                                            juce::Colours::limegreen, m_rectangle.getX());
    }else{
        m_colourGradient = juce::ColourGradient::horizontal(juce::Colours::black, m_rectangle.getX(),
                                                        juce::Colours::limegreen, m_rectangle.getRight());
    }
}
