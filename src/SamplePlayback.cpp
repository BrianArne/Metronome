/*
  ==============================================================================

    SamplePlayback.cpp
    Created: 3.20.2022
    Author:  Brian Arne

  ==============================================================================
*/

#include "SamplePlayback.h"

SamplePlayback::SamplePlayback(std::unique_ptr<juce::AudioBuffer<float>> sampleBuffer, std::atomic<double>& gain, std::atomic<double>& beatPercentage, std::atomic<bool>& reversed)
    : mClickSoundBuffer(std::move(sampleBuffer))
    , mIsPlaying(false)
    , mSampleIndex(0)
    , mSamplesAccumulated(0)
    , mSamplesBetweenClicks(0)
    , mGain(gain)
    , mBeatPercentage(beatPercentage)
    , mReversed(reversed)
{
}

void SamplePlayback::processBuffer(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto numOutChannels = bufferToFill.buffer->getNumChannels();
    
    auto numSamples = bufferToFill.numSamples;
    auto outputSamplesOffset = bufferToFill.startSample;
    
    while (numSamples > 0) {
        if (!mIsPlaying && mSamplesAccumulated == mSamplesBetweenClicks) {
            mIsPlaying = true;
            mSamplesAccumulated = 0;
            auto localReversed = mReversed.load();
            mReversed = !localReversed;
        }
        
        for ( auto channel = 0; channel < numOutChannels; ++channel) {
            if (mIsPlaying) {
                *(bufferToFill.buffer->getWritePointer(channel, outputSamplesOffset)) = *(mClickSoundBuffer->getReadPointer(0, mSampleIndex)) * static_cast<double>(mGain.load());
            } else {
                *(bufferToFill.buffer->getWritePointer(channel, outputSamplesOffset)) = 0;
            }
        }
        if (mIsPlaying) {
            mSampleIndex++;
        }
        
        mSamplesAccumulated++;
        outputSamplesOffset++;
        numSamples--;
        
        // Update mIsPlaying
        if (mSampleIndex == mClickSoundBuffer->getNumSamples()-1) {
            mIsPlaying = false;
            mSampleIndex = 0;
        }
    }
    mBeatPercentage = static_cast<double>(mSamplesAccumulated) / static_cast<double>(mSamplesBetweenClicks);
}

void SamplePlayback::setSampleRate(const double sampleRate)
{
    if (mSampleRate == sampleRate)
        return;
    mSampleRate = sampleRate;
}

void SamplePlayback::tempoChanged(const int newTempo)
{
    mSamplesBetweenClicks = samplesPerClick(newTempo);
    mSampleIndex = 0;
    mSamplesAccumulated = 0;
    mBeatPercentage = 0.0;
    mReversed = false;
    mIsPlaying = false;
}

void SamplePlayback::resetSamplePlayback()
{
    mSampleIndex = 0;
    mSamplesAccumulated = 0;
    mBeatPercentage = 0.0;
    mReversed = false;
    mIsPlaying = false;
}


int SamplePlayback::samplesPerClick(const int tempo)
{
    double secondsPerClick = 60.f / static_cast<double>(tempo);
    return static_cast<int>(mSampleRate * secondsPerClick);
}
