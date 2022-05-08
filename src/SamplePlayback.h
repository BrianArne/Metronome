/*
  ==============================================================================

    SamplePlayback.h
    Created: 3.20.2022
    Author:  Brian Arne

  ==============================================================================
*/

#include <juce_audio_basics/juce_audio_basics.h>

class SamplePlayback {
public:
    SamplePlayback(std::unique_ptr<juce::AudioBuffer<float>> sampleBuffer, const int sampleRate);
    ~SamplePlayback() override;
   
    //==============================================================================

    bool processBuffer(juce::AudioSourceChannelInfo& bufferToFill);
    void tempoChanged(const int newTempo);
    
private:
    //==============================================================================

    void samplesPerClick(const int tempo);
    
    //==============================================================================
    
    using AudioBufferPtr = std::unique_ptr<juce::AudioBuffer<float>>;
    AudioBufferPtr mClickSoundBuffer;
    int mCurrentSample;
    int mSamplesBetweenClicks;
    int mSampleRate;
};

