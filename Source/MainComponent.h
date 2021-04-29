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
                        public juce::Slider::Listener
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
    void sliderValueChanged(juce::Slider* slider) override;
    

private:
    //==============================================================================
    // Your private member variables go here...
    MetronomeLookAndFeel m_lookAndFeel;

    juce::Slider gainSlider;
    juce::Slider tempoSlider;
    std::atomic<int> tempo;
    std::atomic<int> gain;
    
    PongComponent pongDisplay;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
