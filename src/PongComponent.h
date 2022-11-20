/*
  ==============================================================================

    PongComponent.h
    Created: 10 Apr 2021 5:40:16pm
    Author:  Brian Arne

  ==============================================================================
*/

#pragma once
#include <juce_gui_extra/juce_gui_extra.h>

//=============================================================================_=
class PongComponent : public juce::AnimatedAppComponent
{

public:
    enum State{
        STOPPED,
        STARTING,
        PLAYING,
        STOPPING
    };

    PongComponent(std::atomic<double>& beatPercentage, std::atomic<bool>& reversed);
    
    void paint(juce::Graphics& G) override;
    void update() override;
    void changeState(State state);
    int calcX();
    PongComponent::State getState();
    
    class MovingGradient{
        public:
        
            MovingGradient();
            ~MovingGradient() = default;
        
            juce::Rectangle<int> getRectangle();
            void updateRectangle(int x, int y, int width, int height, bool isReversed);
            void setRectangle(juce::Rectangle<int> rec, bool isReversed);
            juce::ColourGradient getColourGradient();

        private:
            void setColourGradient(bool isReversed);
        
            juce::Rectangle<int> mRectangle;
            juce::ColourGradient mColorGradient;
    };
    
private:
    //==============================================================================
    State mNewState;
    MovingGradient mGradient;
    juce::Colour mPaintColor;
    std::atomic<double>& mBeatPercentage;
    std::atomic<bool>& mReversed;
    int mXCoordinate;
};
