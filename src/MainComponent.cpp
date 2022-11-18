#include "MainComponent.h"
#include <BinaryData.h>

//==============================================================================

namespace {
std::unique_ptr<juce::AudioBuffer<float>> createSampleBuffer(juce::WavAudioFormat& wavFormat) {
    auto* inputStream  = new juce::MemoryInputStream(BinaryData::Click_wav, BinaryData::Click_wavSize, false);
    std::unique_ptr<juce::AudioFormatReader> reader(wavFormat.createReaderFor(inputStream, false));
    auto clickBuffer = std::make_unique<juce::AudioBuffer<float>>(static_cast<int>(reader->numChannels), static_cast<int>(reader->lengthInSamples));
    
    clickBuffer->clear();
    reader->read(clickBuffer.get(), 0, clickBuffer->getNumSamples(), 0, true, true);
    return clickBuffer;
}
}

MainComponent::MainComponent() : mSamplePlayback(createSampleBuffer(mFormat))
{
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
    mSamplePlayback.tempoChanged(120);
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
void MainComponent::prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate)
{
    mSamplePlayback.setSampleRate(sampleRate);
    // Tempo isn't loaded yet by the time prepareToPlay is called()
    mSamplePlayback.tempoChanged(120);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (mPlayState == PongComponent::State::STOPPED)
        return;
    mSamplePlayback.processBuffer(bufferToFill);
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

bool MainComponent::keyPressed(const juce::KeyPress &key, juce::Component* /*originatingComponent*/)
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
                mPlayState = PongComponent::State::PLAYING;
                mPlayButton.setButtonText("Stop");
                break;
            case PongComponent::State::PLAYING:
                mSamplePlayback.resetSamplePlayback();
                mPongDisplay.changeState(PongComponent::State::STOPPING);
                mPlayState = PongComponent::State::STOPPED;
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
    // TODO: We can remove here
    //mSamplesPerClick = samplesPerClick(mTempo);
    mSamplePlayback.tempoChanged(mTempo.load());
}
