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
    using juce::KeyListener::keyPressed;
    bool keyPressed (const juce::KeyPress &key, juce::Component *originatingComponent) override;
    bool keyPressed (const juce::KeyPress &key) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    void labelTextChanged(juce::Label *labelThatHasChanged) override;
    int samplesPerClick(const int tempo);

private:
    //==============================================================================
    
    // AUDIO
    juce::WavAudioFormat mFormat;
    juce::AudioBuffer<float> mClickBuffer;
    int mPosition;

    
    //GUI
    MetronomeLookAndFeel mLookAndFeel;
    
    juce::TextButton mPlayButton;
    
    juce::Slider mGainSlider;
    juce::Label mTempoLabel;
    
    std::atomic<int> mTempo;
    std::atomic<double> mGain;
    double mSampleRate;
    int mSampleAccumulator;
    int mSamplesPerClick;
    int mClickSampleIndex;
    
    PongComponent mPongDisplay;
    SamplePlayback mSamplePlayback;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
