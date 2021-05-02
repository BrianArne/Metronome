/*
  ==============================================================================

    PongComponent.cpp
    Created: 10 Apr 2021 5:40:16pm
    Author:  Brian Arne

  ==============================================================================
*/

#include "PongComponent.h"

PongComponent::PongComponent()
{
    setSize(75,200);
    setFramesPerSecond(100);
    initializeGradientArea();
}

void PongComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    // TODO: Check if m_state.m_isPlaying to see if we needed to update the visual. If not just exit

    // Checks if we need to reverse the moving gradient
    auto lastRec = m_gradient.getRectangle();
    if(lastRec.getRight() == getLocalBounds().getRight()){
        m_state.m_isReversed = true;
    }else if(lastRec.getX() == 0){
        m_state.m_isReversed = false;
    }
    
    // Updates the gradient
    m_gradient.updateRectangle(m_state, lastRec.getX(), lastRec.getY(), 10, lastRec.getHeight());
    g.setGradientFill(m_gradient.getColourGradient());
    g.fillRect(m_gradient.getRectangle());
}

void PongComponent::update()
{
    
}

void PongComponent::initializeGradientArea()
{
    auto localBounds = getLocalBounds();
    m_gradient.setRectangle(juce::Rectangle<int>(localBounds.getX(), localBounds.getY(), 5, localBounds.getHeight()));
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

void PongComponent::MovingGradient::updateRectangle(PongState& m_state, int newX, int newY, int newWidth, int newHeight)
{
    if(m_state.m_isReversed){
        m_rectangle.setBounds(newX-1, newY, newWidth, newHeight);
    }else{
        m_rectangle.setBounds(newX+1, newY, newWidth, newHeight);
    }
    setColourGradient(m_state);
}

void PongComponent::MovingGradient::setRectangle(PongState& m_state, juce::Rectangle<int> rec)
{
    m_rectangle = rec;
    setColourGradient(m_state);
}

/*
    @Returns the ColourGradient used to draw the rectangle space
 */
juce::ColourGradient PongComponent::MovingGradient::getColourGradient()
{
    return m_colourGradient;
}

/*
    Private method that is called from updateRectangle(PongState, int, int ,int ,int). When a rectangle is updated we want to keep the
    ColourGradient in sync so it is called with the same state and uses the m_rectangle that was just set by updateRectangle()
 
    Gradient will fade to black in a different direction depending on what if the m_state is reversed(right->left) or not (left->right)
 */
void PongComponent::MovingGradient::setColourGradient(PongState& m_state)
{
    if(m_state.m_isReversed){
        m_colourGradient = juce::ColourGradient::horizontal(juce::Colours::black, m_rectangle.getRight(),
                                                            juce::Colours::mediumvioletred, m_rectangle.getX());
    }else{
        m_colourGradient = juce::ColourGradient::horizontal(juce::Colours::black, m_rectangle.getX(),
                                                        juce::Colours::mediumvioletred, m_rectangle.getRight());
    }
}
