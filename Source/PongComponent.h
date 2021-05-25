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
     Enum representing the state the metronome can be in

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
     
     Framerate of 100.
     */
    PongComponent();
    virtual ~PongComponent() = default;
    
    //==============================================================================
    /**
     Paints the PongComponent. Manages the MovingGradient can change the PongState owned by the PongComponent. Makes calls to updateRectangle()
     to alter the position of the MovingGradient.
     
     Coloring of the component happens here as well.
     */
    void paint(juce::Graphics& G) override;
   
    /**
     @See AnimatedAppComponent::update()
     */
    void update() override;
    
    /**
     Called when the metronome state is changed to STARTING so the moving gradient can be initialized
     */
    void initializeGradientArea();
   
    /**
     Called when the state of the metronome is changed.
     
     @param state    the state the metronome will be changed to
     */
    void changeState(State state);
   
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
             Potential solution to for creating a softer 'bounce' of a MovingGradient().
             */
            void reduceGradientSize();
        
            /**
             Potential solution to for creating a softer 'bounce' of a MovingGradient().
             */
            void increaseGradientSize();
            
            /**
             Returns the m_rectangle that represents the Gradient space being drawn
             */
            juce::Rectangle<int> getRectangle();
        
            /**
             Called by the paint() function of the owning component the MovingGradient to move the drawn gradient in a timely fashion. Calling this fuction
             will call setColourGradient() as well to keep the space being drawn and the gradient space being drawn in sync.
             */
            void updateRectangle(int newX, int newY, int newWidth, int newHeight, bool isReversed);
        
            /**
             Sets the m_Rectangle to a supplied rectangle space. Needs a PongState as well for when this function calls setColourGradient()

             @param PongState&    State of the Pong component
             @param rec  A rectangle representing the space the MovingGradient should be drawn withing
             false, it will stay above it.
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
            int m_gradientWidth;
    };
    
private:
    //==============================================================================
    MovingGradient m_gradient;
    juce::Colour m_paintColour;
    State m_newState;
    bool m_isReversed;
};
