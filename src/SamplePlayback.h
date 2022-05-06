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
    
    SamplePlayback(const juce::AudioBuffer<float> sampleBuffer);
    ~SamplePlayback();
    
    bool processBuffer(juce::AudioBuffer<float>& buffer);
    bool updateTempo(int tempo);
    
private:
    void updateSample();
    
    int mSample;
    
    
    
};

