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
    SamplePlayback(std::unique_ptr<juce::AudioBuffer<float>> sampleBuffer);
    ~SamplePlayback() = default;
   
    //==============================================================================
    void processBuffer(const juce::AudioSourceChannelInfo& bufferToFill);
    void setSampleRate(const double sampleRate);
    void tempoChanged(const int newTempo);
    void resetSamplePlayback();
    
private:
    //==============================================================================
    int samplesPerClick(const int tempo);
    
    //==============================================================================
    using AudioBufferPtr = std::unique_ptr<juce::AudioBuffer<float>>;
    AudioBufferPtr mClickSoundBuffer;
    double mSampleRate;
    int mSampleIndex;
    int mSamplesAccumulated;
    int mSamplesBetweenClicks;
};
