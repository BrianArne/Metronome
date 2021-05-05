#pragma once

#include <JuceHeader.h>
#include "MetronomeLookAndFeel.h"
#include "PongComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  :  public juce::AudioAppComponent,
                        public juce::Slider::Listener,
                        public juce::Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    //==============================================================================
    /**     TODO: Document this
     <Genearl idea of function>
     
     Pre,Post considerattions
     
     @param <name>    function
     ...
     @param onLeft   if true, the label will stay on the left of its component; if
     false, it will stay above it.
     */
    void sliderValueChanged(juce::Slider* slider) override;
    
    /**     TODO: Document this
     <Genearl idea of function>
     
     Pre,Post considerattions
     
     @param <name>    function
     ...
     @param onLeft   if true, the label will stay on the left of its component; if
     false, it will stay above it.
     */
    void buttonClicked(juce::Button* button) override;
    

private:
    //==============================================================================
    // Your private member variables go here...
    MetronomeLookAndFeel m_lookAndFeel;
    
    juce::TextButton m_playButton;
    
    juce::Slider m_gainSlider;
    juce::Slider m_tempoSlider;
    std::atomic<int> m_tempo;
    std::atomic<int> m_gain;
    
    PongComponent pongDisplay;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
