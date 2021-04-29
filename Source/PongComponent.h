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


class PongComponent : public juce::AnimatedAppComponent
{

public:
    PongComponent();
    ~PongComponent();
    
    void paint(juce::Graphics& G) override;
    void update() override;
    
    // TODO: remove initializeLines();
    void initializeLines();
    void initializeGradientArea();
    
    void moveBar();
    // TODO: Re-evaluate if we need to change to moveBar()
    // Add other functions to be called in this one for beginning(), end(), reverse() for dealing with dimishing rec length
    void moveBar(const int& rightBoundLimit);
    

    // TODO: Change to moving gradient??
    class MovingLine{
       
        // TODO: Implementation needs
        // Need methods to deal with the beginning and ending and reversing of the moving gradient
        // Member vars should be rectangle size
        public:
            MovingLine(juce::Line<float>& line, juce::Colour colour);
            ~MovingLine();
            
            // TODO: Can remove
            void setBrightness(const u_short& brightness);
            u_short getBrightness();
        
            // TODO: Can remove
            void darker();
        
            void setColour(juce::Colour colour);
            juce::Colour getColour();
        
            // TODO: Can remove
            juce::Line<float> getLine();
            void setLine(juce::Line<float>& line);
        
            // TODO: Can remove
            void makeAvailable();
            void makeUnavailable();
            bool isAvailable();
        
        private:
            juce::Line<float> m_line;
            juce::Colour m_colour;
            u_short m_brightness;
            bool m_available;
    };
    
    class MovingGradient{
        
        public:
            MovingGradient();
            MovingGradient(juce::Rectangle<int> rec);
            ~MovingGradient();
        
            // I think we can do this better without reducing
            void reduceGradientSize();
            void increaseGradientSize();
            
            juce::Rectangle<int> getRectangle();
            void updateRectangle(PongState& m_state, int newX, int newY, int newWidth, int newHeight);
            void setRectangle(juce::Rectangle<int> rec);
            
            juce::ColourGradient getColourGradient();
            void setColourGradient(PongState& m_state);
            void setColourGradient(juce::ColourGradient c);

        private:
            juce::Rectangle<int> m_rectangle;
            juce::ColourGradient m_colourGradient;
            int m_gradientWidth;
    };
    
    
private:
    //TODO: Can remove, change to MovingGradient implementation
    std::vector<MovingLine> m_lines;
    
    MovingGradient m_gradient;
    juce::Colour m_paintColour;
    PongState m_state;
};
