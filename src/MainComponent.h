#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "MetronomeLookAndFeel.h"
#include "PongComponent.h"
#include "SamplePlayback.h"

class MainComponent  :  public juce::AudioAppComponent,
                        public juce::KeyListener,
                        public juce::Label::Listener,
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
    using juce::Component::keyPressed;
    bool keyPressed (const juce::KeyPress &key, juce::Component *originatingComponent) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    void labelTextChanged(juce::Label *labelThatHasChanged) override;

private:
    //==============================================================================
    
    // AUDIO
    juce::WavAudioFormat mFormat;
    PongComponent::State mPlayState;
    
    //GUI
    MetronomeLookAndFeel mLookAndFeel;
    juce::TextButton mPlayButton;
    juce::Slider mGainSlider;
    juce::Label mTempoLabel;
    
    std::atomic<double> mGain;
    juce::AudioBuffer<float> mClickBuffer;
    std::atomic<int> mTempo;
    
    PongComponent mPongDisplay;
    SamplePlayback mSamplePlayback;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
