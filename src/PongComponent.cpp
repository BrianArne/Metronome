/*
  ==============================================================================

    PongComponent.cpp
    Created: 10 Apr 2021 5:40:16pm
    Author:  Brian Arne

  ==============================================================================
*/

#include "PongComponent.h"
#define GRADIENT_WIDTH 50

PongComponent::PongComponent() : mNewState(State::STOPPED), mReversed(false), mTimePassed(0.f), mMillisecPerBeat(0.f)
{
    setFramesPerSecond(500);
    auto localBounds = getLocalBounds();
    mGradient.setRectangle(juce::Rectangle<int>(localBounds.getX(), localBounds.getY(), GRADIENT_WIDTH, localBounds.getHeight()), mReversed);
}

void PongComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    if (getState() == PongComponent::PLAYING){
        if (mJustReversed){
            mGradient.updateRectangle(mXCoordinate, getLocalBounds().getY(), GRADIENT_WIDTH, getLocalBounds().getHeight(), !mReversed);
            mJustReversed =false;
        }else{
            mGradient.updateRectangle(mXCoordinate, getLocalBounds().getY(), GRADIENT_WIDTH, getLocalBounds().getHeight(), mReversed);
        }
        g.setGradientFill(mGradient.getColourGradient());
        g.fillRect(mGradient.getRectangle());
    }
}

void PongComponent::update()
{
    if (getState() == PongComponent::PLAYING){
        mTimePassed += getMillisecondsSinceLastUpdate();
        if (mTimePassed > mMillisecPerBeat) {
            reverse();
        };
    }
    mXCoordinate = calcX();
}

void PongComponent::tempoChanged(const int newTempo)
{
    mMillisecPerBeat = calcMillisecPerBeat(newTempo);
}

float PongComponent::calcMillisecPerBeat(const int tempo)
{
    return (1000.f / (static_cast<float>(tempo) / 60.f));
}

void PongComponent::changeState(State state)
{
    if ( mNewState != state){ // m_newState is a poor naming choice, we should swap state and new state naming
        mNewState = state;
        switch(mNewState)
        {
            case STOPPED:
                mTimePassed = 0.f;
                mReversed = false;
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
    return mNewState;
}

void PongComponent::reverse()
{
    mReversed = !mReversed;
    mTimePassed -= mMillisecPerBeat;
    mJustReversed = true;

}

int PongComponent::calcX()
{
    if (mReversed && mJustReversed){
        return getWidth()-GRADIENT_WIDTH;
    }else if (!mReversed && mJustReversed){
        return 0;
    }

    float percentage = mTimePassed / mMillisecPerBeat;
    if (percentage != 1.f){
        percentage = percentage - static_cast<int>(percentage);
    }
    
    if (mReversed){
        return  getLocalBounds().getRight()-GRADIENT_WIDTH - static_cast<int>((getWidth()-GRADIENT_WIDTH) * percentage);
    }else{
        return static_cast<int>((getWidth()-GRADIENT_WIDTH) * percentage);
    }
}

//==============================================================================

PongComponent::MovingGradient::MovingGradient(){}


juce::Rectangle<int> PongComponent::MovingGradient::getRectangle()
{
    return mRectangle;
}

void PongComponent::MovingGradient::updateRectangle(int x, int y, int width, int height, bool isReversed)
{
    mRectangle.setBounds(x, y, width, height);//}
    setColourGradient(isReversed);
}

void PongComponent::MovingGradient::setRectangle(juce::Rectangle<int> rec, bool isReversed)
{
    mRectangle = rec;
    setColourGradient(isReversed);
}

juce::ColourGradient PongComponent::MovingGradient::getColourGradient()
{
    return mColorGradient;
}

void PongComponent::MovingGradient::setColourGradient(const bool isReversed)
{
    if(isReversed){
        mColorGradient = juce::ColourGradient::horizontal(juce::Colours::black, mRectangle.getRight(),
                                                            juce::Colours::limegreen, mRectangle.getX());
    }else{
        mColorGradient = juce::ColourGradient::horizontal(juce::Colours::black, mRectangle.getX(),
                                                        juce::Colours::limegreen, mRectangle.getRight());
    }
}
