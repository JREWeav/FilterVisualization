#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <JuceHeader.h>
#include "AudioPlayer.h"
#include "Spectrogram.h"
#include "AudioFilter.h"
#include "FilterLine.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class AudioGUI : public juce::AudioAppComponent, public juce::Button::Listener, public juce::Slider::Listener, public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    AudioGUI();

    // Audio
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

    // Graphics/GUI
    void paint(juce::Graphics &) override;
    void resized() override;

    // Listeners
    void buttonClicked(Button *) override;
    void sliderValueChanged(Slider *slider) override;

private:
    //==============================================================================
    // Your private member variables go here...
    TextButton loadBtn{"LOAD"};
    TextButton playBtn{"PLAY"};
    Slider gainSlider;

    FileChooser fileChooser{"LOAD"};

    AudioFormatManager audForManager;

    AudioPlayer audPlayer{&audForManager};
    AudioFilter filter;
    FilterLine filterLine{&filter};

    Spectrogram spectImage;

    Slider lowPassFreq;
    Slider lowPassSlope;

    Slider peakFreq;
    Slider peakGain;
    Slider peakQ;

    Slider highPassFreq;
    Slider highPassSlope;

    bool isPlaying;

    dsp::IIR::Filter<float> lowPassFilterLeft;
    dsp::IIR::Filter<float> lowPassFilterRight;

    float lowPassFreqStored;
    double sampleRateStored;

    Label peakFreqLab;
    Label peakGainLab;
    Label peakQLab;
    
    Label lpFreqLab;
    Label lpSlopeLab;

    Label hpFreqLab;
    Label hpSlopeLab;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioGUI)
};
