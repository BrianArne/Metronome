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

    PongComponent();
    //virtual ~PongComponent() = default;
    
    void paint(juce::Graphics& G) override;
    void update() override;
    void tempoChanged(const int newTempo);
    float calcMillisecPerBeat(const int tempo);
    void changeState(State state);
    void reverse();
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

    bool mReversed;
    bool mJustReversed;
    float mTimePassed; // Accumulated time while playing to determine x coordinate of the MovingGradient
    float mMillisecPerBeat;
    int mXCoordinate;
};
