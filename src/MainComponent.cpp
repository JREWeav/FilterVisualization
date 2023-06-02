#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize(1000, 600);
    setAudioChannels(0, 2); // no inputs, two outputs

    addAndMakeVisible(audGUI);
}

// Audio

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audGUI.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    audGUI.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    audGUI.releaseResources();
}

// Graphics/GUI

void MainComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::skyblue);

    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
    g.drawText("If You See This Everything Is Working!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    audGUI.setBounds(0, 0, getWidth(), getHeight());
}
