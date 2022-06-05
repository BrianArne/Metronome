/*
  ==============================================================================

    SamplePlayback.cpp
    Created: 3.20.2022
    Author:  Brian Arne

  ==============================================================================
*/

#include "SamplePlayback.h"

SamplePlayback::SamplePlayback(std::unique_ptr<juce::AudioBuffer<float>> sampleBuffer)
    : mClickSoundBuffer(std::move(sampleBuffer))
    , mSampleIndex(0)
    , mSamplesAccumulated(0)
    , mSamplesBetweenClicks(0)
{
}

void SamplePlayback::processBuffer(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto numInputChannels = mClickSoundBuffer->getNumChannels();
    auto numOutChannels = bufferToFill.buffer->getNumChannels();
    
    auto numSamples = bufferToFill.numSamples;
    auto outputSamplesOffset = bufferToFill.startSample;
    
    // If number samples to fill plus accumulated still is less that needed samples
    // fill the whole buffer with silence and update AccumulatedSamples
    if (mSamplesAccumulated + numSamples <= mSamplesBetweenClicks){
        bufferToFill.buffer->clear(outputSamplesOffset, numSamples);
        mSamplesAccumulated += numSamples;
        return;
    } else {
        // Fill with silence till we need samples
        int samplesTillClick = mSamplesBetweenClicks - mSamplesAccumulated;
        
        if (!(samplesTillClick < 0) && samplesTillClick < numSamples){
            // TODO: Update accumulated samples
            bufferToFill.buffer->clear(outputSamplesOffset, samplesTillClick);
            outputSamplesOffset += samplesTillClick;
            numSamples -= samplesTillClick;
            mSamplesAccumulated += samplesTillClick;
        }
        
        if (numSamples > 0 ) {
            // TODO: Sample accumulated should return to zero until after
            // mSamplesAccumulated == samplesTillClick + mClickSoundBuffer.getNumSamples();
            
            // Calculate how many samples from clickBuffer to put into audioBuffer
            auto bufferSamplesRemaining = mClickSoundBuffer->getNumSamples() - mSampleIndex;
            
            auto clickSamplesAvailable = juce::jmin(numSamples, bufferSamplesRemaining);
            
            // TODO: Zero out if we have played all the samples we need, and update accumulator
            // TODO: Update accumulated samples
            // Zero out if we have played all we need
            
            for ( auto channel = 0; channel < numOutChannels; ++channel)
            {
                // Changed to channel 0 for destination, but is it 1?
                bufferToFill.buffer->copyFrom(channel, outputSamplesOffset, *(mClickSoundBuffer.get()), channel % numInputChannels, mSampleIndex, clickSamplesAvailable);
            }
            
            // Update state
            // TODO: Update accumulated samples
            mSampleIndex += clickSamplesAvailable;
            numSamples -= clickSamplesAvailable;
            outputSamplesOffset += clickSamplesAvailable;
            //mSampleIndex += clickSamplesAvailable;
            if (mSampleIndex == mClickSoundBuffer->getNumSamples()) {
                mSamplesAccumulated = 0;
                mSampleIndex = 0;
            }
        }
    }
}

void SamplePlayback::setSampleRate(const double sampleRate)
{
    if (mSampleRate == sampleRate)
        return;
    mSampleRate = sampleRate;
}

void SamplePlayback::tempoChanged(const int newTempo)
{
    // This should be a pure function
    mSamplesBetweenClicks = samplesPerClick(newTempo);
    mSampleIndex = 0;
    mSamplesAccumulated = 0;
}

void SamplePlayback::resetSamplePlayback()
{
    mSampleIndex = 0;
    mSamplesAccumulated = 0;
}


int SamplePlayback::samplesPerClick(const int tempo)
{
    double secondsPerClick = 60.f / static_cast<double>(tempo);
    return static_cast<int>(mSampleRate * secondsPerClick);
}
