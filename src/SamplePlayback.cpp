/*
  ==============================================================================

    SamplePlayback.cpp
    Created: 3.20.2022
    Author:  Brian Arne

  ==============================================================================
*/

#include "SamplePlayback.h"

SamplePlayback::SamplePlayback(std::unique_ptr<juce::AudioBuffer<float>> sampleBuffer, const int sampleRate) : mClickSoundBuffer(std::move(sampleBuffer)),
    mSampleRate(sampleRate)
{
}

bool SamplePlayback::processBuffer(juce::AudioSourceChannelInfo& bufferToFill)
{
    /*
   // auto numInputChannels = mClickBuffer.getNumChannels();
    auto numInputChannels = mClickSoundBuffer->getNumChannels();
    
    auto numOutChannels = bufferToFill.buffer->getNumChannels();
    
    auto outputSamplesRemaining = bufferToFill.numSamples;
    auto outputSamplesOffset = bufferToFill.startSample;
    
    while ( outputSamplesRemaining > 0 ){
        auto bufferSamplesRemaining = mClickSoundBuffer->getNumSamples() - mPosition;
        auto samplesThisTime = juce::jmin(outputSamplesRemaining, bufferSamplesRemaining);
        
        for ( auto channel = 0; channel < numOutChannels; ++channel)
        {
            bufferToFill.buffer->copyFrom(channel, outputSamplesOffset, mClickBuffer.get(), channel % numInputChannels, mPosition, samplesThisTime);
        }
        outputSamplesRemaining -= samplesThisTime;
        outputSamplesOffset += samplesThisTime;
        mPosition += samplesThisTime;
        
        if ( mPosition == mClickBuffer->getNumSamples() )
        {
            mPosition = 0;
        }
    }
   */
}

void SamplePlayback::tempoChanged(const int newTempo)
{
    samplesPerClick(newTempo);
}


void SamplePlayback::samplesPerClick(const int tempo)
{
    double secondsPerClick = static_cast<double>(tempo) / 60.f;
    mSamplesBetweenClicks = static_cast<int>(mSampleRate * secondsPerClick);
}
