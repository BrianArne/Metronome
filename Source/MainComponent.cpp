#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (300, 400);
   
    m_gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    m_gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_gainSlider.setRange(-50, 50, 0.5);
    m_gainSlider.setLookAndFeel(&m_lookAndFeel);

    m_tempoLabel.setText("120", juce::NotificationType::dontSendNotification);
    m_tempoLabel.setJustificationType(juce::Justification::centred);
    m_tempoLabel.setFont(juce::Font(140));
    m_tempoLabel.setEditable(false);
    m_tempoLabel.addListener(this);
    m_tempoLabel.setLookAndFeel(&m_lookAndFeel);
    m_pongDisplay.tempoChanged(120);

    m_playButton.setButtonText("Play");
    m_playButton.setLookAndFeel(&m_lookAndFeel);
    m_playButton.addListener(this);
    
    addKeyListener(this);

    addAndMakeVisible(m_gainSlider);
    addAndMakeVisible(m_tempoLabel);
    addAndMakeVisible(m_pongDisplay);
    addAndMakeVisible(m_playButton);

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
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
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

    m_gainSlider.setBounds(topLeftGain);
    m_tempoLabel.setBounds(topHalf);
    m_playButton.setBounds(localRec.removeFromTop(localRec.getHeight()/3));
    m_pongDisplay.setBounds(localRec);
}

//==============================================================================

bool MainComponent::keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent)
{
    // TODO: Doe we need to change to getIntValue() from the label? Why do we have floats where?
    float labelVal = m_tempoLabel.getText().getFloatValue();
    if (key == juce::KeyPress::upKey){
        m_pongDisplay.changeState(PongComponent::State::STOPPING);
        m_playButton.setButtonText("Play");
        if (labelVal >= 296.f){
            m_tempoLabel.setText(juce::String(300.f), juce::NotificationType::sendNotification);
        }else{
            m_tempoLabel.setText(juce::String(labelVal+5.0f), juce::NotificationType::sendNotification);
        }
        return true;
    }else if (key == juce::KeyPress::downKey){
        m_pongDisplay.changeState(PongComponent::State::STOPPING);
        m_playButton.setButtonText("Play");
        if (labelVal <= 5.f){
            m_tempoLabel.setText(juce::String(1.f), juce::NotificationType::sendNotification);
        }else{
            m_tempoLabel.setText(juce::String(labelVal-5.0f), juce::NotificationType::sendNotification);
        }
        return true;
    }else if (key == juce::KeyPress::leftKey){
        m_pongDisplay.changeState(PongComponent::State::STOPPING);
        m_playButton.setButtonText("Play");
        if (labelVal <= 1.f){
            m_tempoLabel.setText(juce::String(1.f), juce::NotificationType::sendNotification);
        }else{
            m_tempoLabel.setText(juce::String(labelVal-1.0f), juce::NotificationType::sendNotification);
        }
        return true;
    }else if (key == juce::KeyPress::rightKey){
        m_pongDisplay.changeState(PongComponent::State::STOPPING);
        m_playButton.setButtonText("Play");
        if (labelVal >= 299.f){
            m_tempoLabel.setText(juce::String(300.f), juce::NotificationType::sendNotification);
        }else{
            m_tempoLabel.setText(juce::String(labelVal+1.0f), juce::NotificationType::sendNotification);
        }
        return true;
    }else if (key == juce::KeyPress::spaceKey){
        buttonClicked(&m_playButton);
        return true;
    }
    return false;
}

void MainComponent::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &m_gainSlider) m_gain = slider->getValue();
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if( button == &m_playButton){
        switch (m_pongDisplay.getState())
        {
            case PongComponent::State::STOPPED:
                m_pongDisplay.changeState(PongComponent::State::STARTING);
                m_playButton.setButtonText("Stop");
                break;
            case PongComponent::State::PLAYING:
                m_pongDisplay.changeState(PongComponent::State::STOPPING);
                m_playButton.setButtonText("Play");
                break;
            default:
                break;
        }
    }
}

void MainComponent::labelTextChanged(juce::Label *labelThatHasChanged)
{
    //TODO: update the atomic, push down to PongComponent;
    if (labelThatHasChanged == &m_tempoLabel){
        m_tempo = labelThatHasChanged->getText().getIntValue();
        m_pongDisplay.tempoChanged(m_tempo.load());
    }
}
