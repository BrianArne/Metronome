/*
  ==============================================================================

    PongComponent.cpp
    Created: 10 Apr 2021 5:40:16pm
    Author:  Brian Arne

  ==============================================================================
*/

#include "PongComponent.h"
#define GRADIENT_WIDTH 50

PongComponent::PongComponent(std::atomic<double>& beatPercentage, std::atomic<bool>& reversed) :
    mNewState(State::STOPPED),
    mBeatPercentage(beatPercentage),
    mReversed(reversed)
{
    setFramesPerSecond(60);
    auto localBounds = getLocalBounds();
    mGradient.setRectangle(juce::Rectangle<int>(localBounds.getX(), localBounds.getY(), GRADIENT_WIDTH, localBounds.getHeight()), mReversed.load());
}

void PongComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    if (getState() == PongComponent::PLAYING) {
        mGradient.updateRectangle(mXCoordinate, getLocalBounds().getY(), GRADIENT_WIDTH, getLocalBounds().getHeight(), mReversed.load());
        g.setGradientFill(mGradient.getColourGradient());
        g.fillRect(mGradient.getRectangle());
    }
}

void PongComponent::update()
{
    mXCoordinate = calcX();
}

void PongComponent::changeState(State state)
{
    if (mNewState != state){ // m_newState is a poor naming choice, we should swap state and new state naming
        mNewState = state;
        switch(mNewState)
        {
            case STOPPED:
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

int PongComponent::calcX()
{
    bool max = false;
    // Is this needed?
    if (mBeatPercentage.load() > .95)
        max = true;
    if (mReversed.load()){
        if (max){
            return static_cast<int>((getWidth()-GRADIENT_WIDTH) * 0.0);
        }
        return static_cast<int>((getWidth()-GRADIENT_WIDTH) * (1.0 - mBeatPercentage.load()));
    }else{
        if (max){
            return static_cast<int>((getWidth()-GRADIENT_WIDTH) * 1.0);
        }
        return static_cast<int>((getWidth()-GRADIENT_WIDTH) * mBeatPercentage.load());
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
    mRectangle.setBounds(x, y, width, height);
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
