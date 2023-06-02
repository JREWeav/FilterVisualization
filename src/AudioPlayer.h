#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class AudioPlayer : public juce::AudioSource
{
public:
    //==============================================================================
    AudioPlayer(AudioFormatManager *afm);

    // Audio
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

    // Interaction
    void loadFile(File file);
    void pause();
    void play();
    void setGain(float gain);
    bool isPlaying();

private:
    //==============================================================================
    // Your private member variables go here...
    AudioFormatManager *audioForMan;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    AudioTransportSource audTrans;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPlayer)
};
