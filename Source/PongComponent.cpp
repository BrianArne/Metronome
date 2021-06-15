/*
  ==============================================================================

    PongComponent.cpp
    Created: 10 Apr 2021 5:40:16pm
    Author:  Brian Arne

  ==============================================================================
*/

#include "PongComponent.h"
#define GRADIENT_WIDTH 50

PongComponent::PongComponent() : m_newState(State::STOPPED), m_isReversed(false), m_timePassed(0.f), m_millisecPerBeat(0.f)
{
    setFramesPerSecond(300);
    auto localBounds = getLocalBounds();
    m_gradient.setRectangle(juce::Rectangle<int>(localBounds.getX(), localBounds.getY(), GRADIENT_WIDTH, localBounds.getHeight()), m_isReversed);
}

void PongComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    if (getState() == PongComponent::PLAYING){
        m_gradient.updateRectangle(calcX(), getLocalBounds().getY(), GRADIENT_WIDTH, getLocalBounds().getHeight(), m_isReversed);
        g.setGradientFill(m_gradient.getColourGradient());
        g.fillRect(m_gradient.getRectangle());
    }
}

void PongComponent::update()
{
    if (getState() == PongComponent::PLAYING){
        m_timePassed += getMillisecondsSinceLastUpdate();
        if (m_timePassed > m_millisecPerBeat) {
            reverse();
        };
    }
}

void PongComponent::tempoChanged(const int newTempo)
{
    m_millisecPerBeat = calcMillisecPerBeat(newTempo);
}

float PongComponent::calcMillisecPerBeat(const int tempo)
{
    return (1000.f / (static_cast<float>(tempo) / 60.f));
}

void PongComponent::changeState(State state)
{
    if ( m_newState != state){ // m_newState is a poor naming choice, we should swap state and new state naming
        m_newState = state;
        switch(m_newState)
        {
            case STOPPED:
                m_timePassed = 0.f;
                m_isReversed = false;
                break;
            case STARTING:
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

void PongComponent::reverse()
{
    m_isReversed = !m_isReversed;
    m_timePassed -= m_timePassed;
}

int PongComponent::calcX()
{
    float percentage = m_timePassed / m_millisecPerBeat;
    percentage = percentage - static_cast<int>(percentage);
    if (m_isReversed){
        return  getLocalBounds().getRight()-GRADIENT_WIDTH - static_cast<int>((getWidth()-GRADIENT_WIDTH) * percentage);
    }else{
        return static_cast<int>((getWidth()-GRADIENT_WIDTH) * percentage);
    }
}

//==============================================================================

PongComponent::MovingGradient::MovingGradient(){}


juce::Rectangle<int> PongComponent::MovingGradient::getRectangle()
{
    return m_rectangle;
}

void PongComponent::MovingGradient::updateRectangle(int x, int y, int width, int height, bool isReversed)
{
    m_rectangle.setBounds(x, y, width, height);//}
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
