#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (300, 400);
   

    m_gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    m_gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_gainSlider.setRange(-50, 50, 0.5);
    m_gainSlider.setLookAndFeel(&m_lookAndFeel);

    
    m_tempoSlider.setSliderStyle(juce::Slider::IncDecButtons);
    m_tempoSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 200, 200);
    m_tempoSlider.setRange(0, 1000, 1);
    m_tempoSlider.setValue(160, juce::NotificationType::dontSendNotification);
    m_tempoSlider.setLookAndFeel(&m_lookAndFeel);
    
    m_playButton.setButtonText("Play");
    m_playButton.addListener(this);

    addAndMakeVisible(m_gainSlider);
    addAndMakeVisible(m_tempoSlider);
    addAndMakeVisible(pongDisplay);
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
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black);
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll (juce::Colours::black);

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    auto topHalf = getLocalBounds().removeFromTop(getLocalBounds().getHeight() / 2);
    auto topLeftGain = topHalf.removeFromLeft(topHalf.getWidth() / 3);
    
    m_gainSlider.setBounds(topLeftGain);
    m_tempoSlider.setBounds(topHalf);
    pongDisplay.setBounds(getLocalBounds().removeFromBottom(getLocalBounds().getHeight()/1.75));
    m_playButton.setBounds(getLocalBounds().removeFromBottom(getLocalBounds().getHeight()/8));
}

//==============================================================================
void MainComponent::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &m_gainSlider) m_gain = slider->getValue();
    if (slider == &m_tempoSlider) m_tempo = slider->getValue();
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if( button == &m_playButton){
        switch (pongDisplay.getState())
        {
            case PongComponent::State::STOPPED:
                pongDisplay.changeState(PongComponent::State::STARTING);
                m_playButton.setButtonText("Stop");
                break;
            case PongComponent::State::PLAYING:
                pongDisplay.changeState(PongComponent::State::STOPPING);
                m_playButton.setButtonText("Play");
                break;
        }
    }
}
