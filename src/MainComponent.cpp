#include "MainComponent.h"
#include <BinaryData.h>

//==============================================================================

namespace {
juce::AudioBuffer<float> createSampleBuffer(juce::WavAudioFormat& wavFormat) {
    juce::WavAudioFormat m_wavFormat;
    auto* inputStream  = new juce::MemoryInputStream(BinaryData::Click_wav, BinaryData::Click_wavSize, false);
    std::unique_ptr<juce::AudioFormatReader> reader(wavFormat.createReaderFor(inputStream, false));
    juce::AudioBuffer<float> clickBuffer(static_cast<int>(reader->numChannels), static_cast<int>(reader->lengthInSamples));
    clickBuffer.clear();
    reader->read(&clickBuffer, 0, clickBuffer.getNumSamples(), 0, true, true);
    return clickBuffer;
}
}

MainComponent::MainComponent() : mSamplePlayback(createSampleBuffer(mFormat))
{
    
    // TODO: Initaialize SamplePlayback class
    setSize (300, 400);
   
    mGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mGainSlider.setRange(-50, 50, 0.5);
    mGainSlider.setLookAndFeel(&mLookAndFeel);

    mTempoLabel.setText("120", juce::NotificationType::dontSendNotification);
    mTempoLabel.setJustificationType(juce::Justification::centred);
    mTempoLabel.setFont(juce::Font(140));
    mTempoLabel.setEditable(false);
    mTempoLabel.addListener(this);
    mTempoLabel.setLookAndFeel(&mLookAndFeel);
    mPongDisplay.tempoChanged(120);

    mPlayButton.setButtonText("Play");
    mPlayButton.setLookAndFeel(&mLookAndFeel);
    mPlayButton.addListener(this);
    
    addKeyListener(this);

    addAndMakeVisible(mGainSlider);
    addAndMakeVisible(mTempoLabel);
    addAndMakeVisible(mPongDisplay);
    addAndMakeVisible(mPlayButton);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    auto* inputStream  = new juce::MemoryInputStream(BinaryData::Click_wav, BinaryData::Click_wavSize, false);
    std::unique_ptr<juce::AudioFormatReader> reader(mFormat.createReaderFor(inputStream, false));
    juce::AudioBuffer<float> clickBuffer(static_cast<int>(reader->numChannels), static_cast<int>(reader->lengthInSamples));
    clickBuffer.clear();
    reader->read(&clickBuffer, 0, clickBuffer.getNumSamples(), 0, true, true);
    mClickBuffer.makeCopyOf(clickBuffer);
    mPosition = 0;
    
    // TODO: Check if we need this
    mSampleRate = sampleRate;
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    
    // NEEDED CLASSES
    //
    // Sample/time based wave file player
    // Determines time that we play a sampled sound.
    //  -Should it hold the met file?
    //  - Connection to slider val how? Listener to slider?
    //
    //
    
    auto numInputChannels = mClickBuffer.getNumChannels();
    auto numOutChannels = bufferToFill.buffer->getNumChannels();
    
    auto outputSamplesRemaining = bufferToFill.numSamples;
    auto outputSamplesOffset = bufferToFill.startSample;
    
    while ( outputSamplesRemaining > 0 ){
        auto bufferSamplesRemaining = mClickBuffer.getNumSamples() - mPosition;
        auto samplesThisTime = juce::jmin(outputSamplesRemaining, bufferSamplesRemaining);
        
        for ( auto channel = 0; channel < numOutChannels; ++channel)
        {
            bufferToFill.buffer->copyFrom(channel, outputSamplesOffset, mClickBuffer, channel % numInputChannels, mPosition, samplesThisTime);
        }
        outputSamplesRemaining -= samplesThisTime;
        outputSamplesOffset += samplesThisTime;
        mPosition += samplesThisTime;
        
        if ( mPosition == mClickBuffer.getNumSamples() )
        {
            mPosition = 0;
        }
    }
    
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void MainComponent::resized()
{
    auto localRec = getLocalBounds();
    auto topHalf = localRec.removeFromTop((getLocalBounds().getHeight() / 2) + (getLocalBounds().getHeight() / 8));
    auto topLeftGain = topHalf.removeFromLeft(topHalf.getWidth() / 6);

    mGainSlider.setBounds(topLeftGain);
    mTempoLabel.setBounds(topHalf);
    mPlayButton.setBounds(localRec.removeFromTop(localRec.getHeight()/3));
    mPongDisplay.setBounds(localRec);
}

//==============================================================================

bool MainComponent::keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent)
{
    // TODO: Doe we need to change to getIntValue() from the label? Why do we have floats where?
    float labelVal = mTempoLabel.getText().getFloatValue();
    if (key == juce::KeyPress::upKey){
        mPongDisplay.changeState(PongComponent::State::STOPPING);
        mPlayButton.setButtonText("Play");
        if (labelVal >= 296.f){
            mTempoLabel.setText(juce::String(300.f), juce::NotificationType::sendNotification);
        }else{
            mTempoLabel.setText(juce::String(labelVal+5.0f), juce::NotificationType::sendNotification);
        }
        return true;
    }else if (key == juce::KeyPress::downKey){
        mPongDisplay.changeState(PongComponent::State::STOPPING);
        mPlayButton.setButtonText("Play");
        if (labelVal <= 5.f){
            mTempoLabel.setText(juce::String(1.f), juce::NotificationType::sendNotification);
        }else{
            mTempoLabel.setText(juce::String(labelVal-5.0f), juce::NotificationType::sendNotification);
        }
        return true;
    }else if (key == juce::KeyPress::leftKey){
        mPongDisplay.changeState(PongComponent::State::STOPPING);
        mPlayButton.setButtonText("Play");
        if (labelVal <= 1.f){
            mTempoLabel.setText(juce::String(1.f), juce::NotificationType::sendNotification);
        }else{
            mTempoLabel.setText(juce::String(labelVal-1.0f), juce::NotificationType::sendNotification);
        }
        return true;
    }else if (key == juce::KeyPress::rightKey){
        mPongDisplay.changeState(PongComponent::State::STOPPING);
        mPlayButton.setButtonText("Play");
        if (labelVal >= 299.f){
            mTempoLabel.setText(juce::String(300.f), juce::NotificationType::sendNotification);
        }else{
            mTempoLabel.setText(juce::String(labelVal+1.0f), juce::NotificationType::sendNotification);
        }
        return true;
    }else if (key == juce::KeyPress::spaceKey){
        buttonClicked(&mPlayButton);
        return true;
    }
    return false;
}

bool MainComponent::keyPressed(const juce::KeyPress &key)
{
    return true;
}

void MainComponent::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &mGainSlider) mGain = slider->getValue();
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if( button == &mPlayButton){
        switch (mPongDisplay.getState())
        {
            case PongComponent::State::STARTING:
                break;
            case PongComponent::State::STOPPED:
                mPongDisplay.changeState(PongComponent::State::STARTING);
                mPlayButton.setButtonText("Stop");
                break;
            case PongComponent::State::PLAYING:
                mPongDisplay.changeState(PongComponent::State::STOPPING);
                mPlayButton.setButtonText("Play");
                break;
            case PongComponent::State::STOPPING:
                break;
            default:
                break;
        }
    }
}

void MainComponent::labelTextChanged(juce::Label *labelThatHasChanged)
{
    //TODO: update the atomic, push down to PongComponent;
    if (labelThatHasChanged == &mTempoLabel){
        mTempo = labelThatHasChanged->getText().getIntValue();
        mPongDisplay.tempoChanged(mTempo.load());
    }
    mSamplesPerClick = samplesPerClick(mTempo);
}

int MainComponent::samplesPerClick(const int tempo){
    double secondsPerClick = static_cast<double>(tempo) / 60.f;
    return static_cast<int>(mSampleRate * secondsPerClick);
}
