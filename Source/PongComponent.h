/*
  ==============================================================================

    PongComponent.h
    Created: 10 Apr 2021 5:40:16pm
    Author:  Brian Arne

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PongState.h"


/**
 TODO: Document this
 <A class for....>
 
 Important methods and functionality
 
 @see <optional><Related Classes or Methods>
 */
//==============================================================================
class PongComponent : public juce::AnimatedAppComponent
{

public:
    //==============================================================================
    /**
     Constructor on a PongComponent.
     
     Defaults component size to 75 X 200 with a framerate of 100. Calls initializeGradientArea() to set up the MovingGradient owned by the PongComponent
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
     On creation of the PongComponent, this is called to set the m_gradient space to be a rectangle fully to the left occupying 5pc of space
     */
    void initializeGradientArea();
   
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
            void updateRectangle(PongState& m_state, int newX, int newY, int newWidth, int newHeight);
        
            /**
             Sets the m_Rectangle to a supplied rectangle space. Needs a PongState as well for when this function calls setColourGradient()

             @param PongState&    State of the Pong component
             @param rec  A rectangle representing the space the MovingGradient should be drawn withing
             false, it will stay above it.
             */
            void setRectangle(PongState& m_state, juce::Rectangle<int> rec);
            
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
            void setColourGradient(PongState& m_state);

            juce::Rectangle<int> m_rectangle;
            juce::ColourGradient m_colourGradient;
            int m_gradientWidth;
    };

private:
    //==============================================================================
    MovingGradient m_gradient;
    juce::Colour m_paintColour;
    PongState m_state;
};
