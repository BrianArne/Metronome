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
    
    // Must be after setting component size
    // TODO: Remove
    initializeLines();
    initializeGradientArea();
    //auto holder = getLocalBounds().getX(); // getX initially is 0;
}

PongComponent::~PongComponent()
{
    
}

void PongComponent::paint(juce::Graphics& g)
{
    // Sets the background for the component
    auto lastRec = m_gradient.getRectangle();
    if(lastRec.getRight() == getLocalBounds().getRight()){
        m_state.m_isReversed = true;
    }else if(lastRec.getX() == 0){
        m_state.m_isReversed = false;
    }
    
    m_gradient.updateRectangle(m_state, lastRec.getX(), lastRec.getY(), 10, lastRec.getHeight());
    g.fillAll(juce::Colours::black);
    
    // TODO: Check if m_state.m_isPlaying to see if we needed to update the visual. If not just exit

    //auto recColourGradient = juce::ColourGradient::horizontal(juce::Colours::black, getLocalBounds().getX(),juce::Colours::whitesmoke, getLocalBounds().getX()+4);

    g.setGradientFill(m_gradient.getColourGradient());
    //juce::Rectangle<int> rec(getLocalBounds().getX(), getLocalBounds().getY(), getLocalBounds().getX()+7, getLocalBounds().getHeight());
    // One width test
    //juce::Rectangle<int> rec(getLocalBounds().getX(), getLocalBounds().getY(), getLocalBounds().getX()+4, getLocalBounds().getHeight());

    g.fillRect(m_gradient.getRectangle());
    
    
    //auto recTwoColourGradient = juce::ColourGradient::horizontal(juce::Colours::black, getLocalBounds().getRight(), juce::Colours::whitesmoke, getLocalBounds().getRight() - 50);
    //g.setGradientFill(recTwoColourGradient);
    //g.fillAll();

    /* Used to see if we can do it with native juce::Line functionality */
    /*
    juce::Path testPath;
    juce::Line<float> testSolidLine(static_cast<float>(m_state.m_previousBar),
                                    static_cast<float>(getLocalBounds().getY()),
                                    static_cast<float>(m_state.m_previousBar),
                                    static_cast<float>(getLocalBounds().getBottom()));
    moveBar(getLocalBounds().getRight());
    testPath.addLineSegment(testSolidLine, 5.0f);
    g.fillPath(testPath);
     */
    
    // TODO: Make the PongComponent::Vector<T> work with juce::Path and juce::Line
    // path.clear() can reset a path completely
    // path.addLineSegment() can add a line
    
    

    // TODO: Implement loopback
    /*
    std::vector<MovingLine>::iterator nextBar;
    for(auto it = m_.begin(); it != m_bars.end(); it++){
        if (it->isAvailable()){
            //auto find_it_count = it - m_bars.begin();
            nextBar = it;
            break;
        }
    }
     */
}

void PongComponent::update()
{
    
}

void PongComponent::initializeLines()
{
    // TODO: Change to the new juce::Path // juce::Line approach
    for (int i = 0; i < 16; i++){
        // TODO: Make a juce::Line here for the constructor
        juce::Line<float> line(static_cast<float>(m_state.m_previousBar),
                               static_cast<float>(getLocalBounds().getY()),
                               static_cast<float>(m_state.m_previousBar),
                               static_cast<float>(getLocalBounds().getBottom()));
        PongComponent::MovingLine movingLine(line, juce::Colour(juce::Colours::whitesmoke));
        m_lines.push_back(movingLine);
    }
}

void PongComponent::initializeGradientArea()
{
    auto localBounds = getLocalBounds();
    m_gradient.setRectangle(juce::Rectangle<int>(localBounds.getX(), localBounds.getY(), 5, localBounds.getHeight()));
   /*
    auto initColourGradient = juce::ColourGradient::horizontal(juce::Colours::black, m_gradient.getRectangle().getX(),
                                                               juce::Colours::whitesmoke, m_gradient.getRectangle().getRight());
    m_gradient.setColourGradient(initColourGradient);
    */
}

void PongComponent::moveBar()
{
    
}

void PongComponent::moveBar(const int& rightBoundLimit)
{
    // TODO: Add checks to see if we need to reduce() or increase the rectangle/colour gradient width()
    if (m_state.m_previousBar == rightBoundLimit){
        m_state.m_isReversed = true;
    }else if (m_state.m_previousBar == 0){
        m_state.m_isReversed = false;
    }
    
    if (m_state.m_isReversed){
        m_state.m_previousBar--;
    }else{
        m_state.m_previousBar++;
    }
}



//==============================================================================

PongComponent::MovingLine::MovingLine(juce::Line<float>& line, juce::Colour colour) : m_line(line), m_colour(colour)
{
    // TODO: Change to the juce::Path approach
    m_brightness = 15;
    m_available = true;
}

PongComponent::MovingLine::~MovingLine()
{
    
}

/*
    Sets a abstarcted 'brightness' value to a rectangle / moving bar
 */
void PongComponent::MovingLine::setBrightness(const u_short& brightness)
{
    m_brightness = brightness;
}

/*
    @returns a value indicating the brightness of a rectangle / moving bars colour
 */
u_short PongComponent::MovingLine::getBrightness()
{
    return m_brightness;
}

/*
    Darkens the retangle / moving bar updates the brightness of the bar's state
 */
void PongComponent::MovingLine::darker()
{
    m_colour = m_colour.darker();
    m_brightness--;
}

/*
    Sets the colour a rectangle / moving bar will be drawn as
 */
void PongComponent::MovingLine::setColour(juce::Colour colour)
{
    m_colour = colour;
}

/*
    @Returns the colour the bars will be drawn as
 */
juce::Colour PongComponent::MovingLine::getColour()
{
    return m_colour;
}

/*
    @Returns a location of a moving bar
 */
juce::Line<float> PongComponent::MovingLine::getLine()
{
    // TODO: Change to the juce::Path approach
    return m_line;
}

/*
    Sets a moving bar with a new rectangle location
 */
void PongComponent::MovingLine::setLine(juce::Line<float>& line)
{
    // TODO: Change to the juce;:Path approach
    m_line = line;
}

/*
    Marks a rectangle / mo;ving bar available to be drawn again
 */
void PongComponent::MovingLine::makeAvailable()
{
    m_available = true;
}

/*
    Marks a rectangle / moving bar as unavailable to be drawn again
 */
void PongComponent::MovingLine::makeUnavailable()
{
    m_available = false;
}


/*
    @returns a bool indicating is a rectangle / moving bar is available for be drawn again
 */
bool PongComponent::MovingLine::isAvailable()
{
    return m_available;
}


//==============================================================================

/*
    Default Constructor for Build, TODO: Might be able to remove
 */
PongComponent::MovingGradient::MovingGradient()
{

}

/*
    MovingGradient Destructor
 */
PongComponent::MovingGradient::~MovingGradient()
{
    
}

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

/*
    @Returns a juce::Rectangle<int> that is the current space being drawn
 */
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

/*
    Sets the rectangle area that a gradient should be drawn to
 */
void PongComponent::MovingGradient::setRectangle(juce::Rectangle<int> rec)
{
    // When you set the rectangle, should we just set the colourGradient too?
    m_rectangle = rec;
    //setColourGradient();
}

/*
    @Returns the ColourGradient used to draw the rectangle space
 */
juce::ColourGradient PongComponent::MovingGradient::getColourGradient()
{
    return m_colourGradient;
}

void PongComponent::MovingGradient::setColourGradient(PongState& m_state)
{
    // TODO: incorporate Reversed from state somehow
    if(m_state.m_isReversed){
        m_colourGradient = juce::ColourGradient::horizontal(juce::Colours::black, m_rectangle.getRight(),
                                                            juce::Colours::mediumvioletred, m_rectangle.getX());
    }else{
        m_colourGradient = juce::ColourGradient::horizontal(juce::Colours::black, m_rectangle.getX(),
                                                        juce::Colours::mediumvioletred, m_rectangle.getRight());
    }
}

// TODO: Make private function, call this function after setting or changing the rectangle
void PongComponent::MovingGradient::setColourGradient(juce::ColourGradient c)
{
    m_colourGradient = c;
}
