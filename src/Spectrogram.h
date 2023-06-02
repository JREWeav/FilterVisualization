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
class Spectrogram : public juce::AudioAppComponent, public juce::Timer
{
public:
    //==============================================================================
    Spectrogram();

    // Audio
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

    // Graphics/GUI
    void paint(juce::Graphics &) override;
    void resized() override;

    void sampleIntoArray(float sample);

    // Drawing
    void drawSpectrogram();
    void drawSpectrum();

    // Timer
    void timerCallback() override;

private:
    //==============================================================================
    // Your private member variables go here...

    // FFT Objects
    dsp::FFT fft;
    dsp::WindowingFunction<float> window;

    // Image to store spectrogram
    Image spectImage;

    // Order of FFT in this case 11 (2048)
    const static int fftOrder = 11;
    const static int fftSize = 1 << fftOrder;
    const static int scopeSize = 1024;

    // Array to take in the data and then to store the data out
    std::array<float, fftSize> fftDataIn;
    std::array<float, fftSize * 2> fftDataOut;
    std::array<float, fftSize * 2> fftWindowDataOut;
    std::array<float, scopeSize> scopeData;

    // Temporary variables to load lines one at a time
    int fftIndex = 0;
    bool nextFFTBlockReady = false;

    int width = getWidth();
    int height = getHeight();

    Path spectrumCurve;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Spectrogram)
};
