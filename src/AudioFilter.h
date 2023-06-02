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
class AudioFilter : public juce::AudioSource
{
public:
    //==============================================================================
    AudioFilter();

    // Audio
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    // Interaction
    void setLowPassFreq(float freq);
    void setLowPassSlope(int slope);
    void setPeakFreq(float freq);
    void setPeakGain(int gain);
    void setPeakQ(float q);
    void setHighPassFreq(float freq);
    void setHighPassSlope(int slope);
    double getSampleRate();

    // Define some shorthands for our filter Chains

    using LowOrHighPass = dsp::ProcessorChain<dsp::IIR::Filter<float>, dsp::IIR::Filter<float>, dsp::IIR::Filter<float>, dsp::IIR::Filter<float>>; // 4 filters to make one low or highpass because one filter is by default 12db slope so now we can make it up to 48db slope

    using SignalPath = dsp::ProcessorChain<LowOrHighPass, dsp::IIR::Filter<float>, LowOrHighPass>; // 3 points in our IIR filter

    //Public SignalPaths so we can access them from audioGUI
    SignalPath leftPath;
    SignalPath rightPath;

    // Function to calculate Coefficients
    void defineFiltersMakeCoefficients(double sampleRate);

private:
    //==============================================================================
    // Your private member variables go here...

    float lowPassFreq;
    int lowPassSlope;

    float peakFreq;
    float peakGainAsDB;
    float peakQ;

    float highPassFreq;
    int highPassSlope;

    double sampleRateStored;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFilter)
};
