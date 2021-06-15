/*
  ==============================================================================

    PongComponent.h
    Created: 10 Apr 2021 5:40:16pm
    Author:  Brian Arne

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/**
 A class for drawing a moving line that hits the sides of the components at a given interval. Used to represent a metronome pendulum.
 */
//=============================================================================_=
class PongComponent : public juce::AnimatedAppComponent
{

public:
    //==============================================================================
    /**
     Enum representing the state a metronome can be in

     @see changeState();
     */
    enum State{
        STOPPED,
        STARTING,
        PLAYING,
        STOPPING
    };

    /**
     Constructor on a PongComponent.
     
     Framerate of 300.
     */
    PongComponent();
    virtual ~PongComponent() = default;
    
    //==============================================================================
    /**
     Paints the PongComponent. Paints the MovingGradient according to where it should be if playing by caling calcX();

     */
    void paint(juce::Graphics& G) override;
   
    /**
     If the metronome is playing, the m_timePassed is accumulated here.
     
     reverse() will be called if the accumulated time is larger than the alloted millisecPerBeat

     @See AnimatedAppComponent::update()
     */
    void update() override;
    
    /**
     The owning parent component, MainComponent, will call this function when the label is changed by a user. tempoChanged() calls calcMilliSecPerBeat to update the member variable m_millisecPerBeat used in update()
     
     @param newTempo a new tempo value pulled from the tempo label in MainComponent
     */
    void tempoChanged(const int newTempo);
    
    /**
     Calculates millisecPerBeat for a given tempo
     
     @param tempo tempo that we need the milliseconds per beat from
     */
    float calcMillisecPerBeat(const int tempo);
   
    /**
     Called when the state of the metronome is changed.
     
     @param state    The state the metronome will be changed to
     */
    void changeState(State state);
   
    /**
     Reverses the direction of m_isReversed
     
     TODO: Look into this
     updates m_timePassed for some reason
     */
    void reverse();
    
    /**
     Determines where the x coordinate of the MovingGradient rectangle should be
     */
    int calcX();
   
    /**
     Returns the state the metronome is in
     */
    PongComponent::State getState();
    
    /**
     A class that represents a moving line.The line bounces back and forth between a component rectangle striking each side of the rectangle at a user requested interval.

     updateRectangle() is called ot update the position that line should be drawn in. In turn updateRectangle() will call setColourGradient to keep both the gradient and
     rectangel in sync. Colours are defaulted at the moment to a fade from black to juce::Colours::mediumvioletred
     
     @see juce::ColourGradient, juce::Rectangle<int>
     */
    //==============================================================================
    class MovingGradient{
        public:
            //==============================================================================
        
            /**
             Default Constructor for MovingGradient
            
             Currently MovingGradient memberVariables are set by the owning PongComponent calling initializeGradientAre(). This is done so that the
             PongComponent can initialize the MovingGradient after its own size has been initialized.
             */
            MovingGradient();
            ~MovingGradient() = default;
        
            /**
             Returns the m_rectangle that represents the Gradient space being drawn
             */
            juce::Rectangle<int> getRectangle();
        
            /**
             Called by the paint() function of the owning component the MovingGradient to move the drawn gradient in a timely fashion. Calling this fuction
             will call setColourGradient() as well to keep the space being drawn and the gradient space being drawn in sync.
             */
            void updateRectangle(int x, int y, int width, int height, bool isReversed);
        
            /**
             Sets the m_Rectangle to a supplied rectangle space. Needs a PongState as well for when this function calls setColourGradient()

             @param rec  A rectangle representing the space the MovingGradient should be drawn withing
             @param isReversed Determines which directiong the MovingGradient is moving. False == Left->Right, True == Right->Left
             */
            void setRectangle(juce::Rectangle<int> rec, bool isReversed);
            
            /**
             Returns the m_colourGradient drawn by the owning component paint() function
             */
            juce::ColourGradient getColourGradient();

        private:
            //==============================================================================
            /**
             Sets the m_colourGradient according the m_rectangle and the PongState passed to the updateRectangle() or setRectangle()

             @param m_state    PongState that the owning PongComponent is in
             */
            void setColourGradient(bool isReversed);

            juce::Rectangle<int> m_rectangle;
            juce::ColourGradient m_colourGradient;
    };
    
private:
    //==============================================================================
    State m_newState;
    MovingGradient m_gradient;
    juce::Colour m_paintColour;

    bool m_isReversed;
    float m_timePassed; // Accumulated time while playing to determine x coordinate of the MovingGradient
    float m_millisecPerBeat;
};
