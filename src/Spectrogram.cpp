#include "Spectrogram.h"

//==============================================================================
Spectrogram::Spectrogram() : fft(fftOrder),
                             spectImage(juce::Image::RGB, 256, 256, true),
                             window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    startTimerHz(45);
}

// Audio

void Spectrogram::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
}

void Spectrogram::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    if (bufferToFill.buffer->getNumChannels() > 0)
    {
        const float *channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

        for (int i = 0; i < bufferToFill.numSamples; ++i)
        {
            sampleIntoArray(channelData[i]);
        }
    }
}

void Spectrogram::releaseResources()
{
}

// Graphics/GUI

void Spectrogram::paint(juce::Graphics &g)
{
    g.setOpacity(1.0f);
    g.drawImage(spectImage, getLocalBounds().toFloat());

    g.setColour(Colours::yellow);
    g.strokePath(spectrumCurve, PathStrokeType(1.f));
}

void Spectrogram::resized()
{
    height = getHeight();
    width = getWidth();
}

void Spectrogram::sampleIntoArray(float sample)
{
    // If we have enough data in fftDataIn
    if (fftIndex == fftSize)
    {
        if (!nextFFTBlockReady)
        {
            std::fill(fftDataOut.begin(), fftDataOut.end(), 0.0f);             // fill fftDataOut with 0
            std::copy(fftDataIn.begin(), fftDataIn.end(), fftDataOut.begin()); // Copy data into fftDataOut

            fft.performFrequencyOnlyForwardTransform(fftDataOut.data()); // Perform fft on the data

            std::fill(fftWindowDataOut.begin(), fftWindowDataOut.end(), 0.0f);       // fill fftWindowDataOut with 0
            std::copy(fftDataIn.begin(), fftDataIn.end(), fftWindowDataOut.begin()); // Copy data into fftWindowDataOut

            window.multiplyWithWindowingTable(fftWindowDataOut.data(), fftSize); // Window the Data
            fft.performFrequencyOnlyForwardTransform(fftWindowDataOut.data());   // Perform fft on the data

            // Now we have 2 Arrays: One windowed and one not windows for the Spectrum display and the Spectrogram respectively

            nextFFTBlockReady = true;
        }
        fftIndex = 0; // Reset fft Index
    }

    // Make the current index in fftDataIn equal to the sample, increase value of index
    fftDataIn[fftIndex] = sample;
    fftIndex++;
}

void Spectrogram::drawSpectrogram()
{
    int lastRow = spectImage.getWidth() - 1;
    int imgHeight = spectImage.getHeight();

    spectImage.moveImageSection(0, 0, 1, 0, lastRow, imgHeight); // Move image left by 1 pixel

    Range<float> maxLevel = FloatVectorOperations::findMinAndMax(fftDataOut.data(), fftSize / 2); // Calculate a range of the FFT data so we are able to scale the image

    for (int i = 1; i < imgHeight; i++)
    {
        auto skewedProportionY = 1.0f - std::exp(std::log((float)i / (float)imgHeight) * 0.2f); // Logarithmic operation to scale the values to better represent the full range of frequencies

        auto fftDataIndex = jlimit(0, fftSize / 2, (int)(skewedProportionY * fftSize / 2)); // Get the index based off of the skewed value

        float intensity = jmap(fftDataOut[fftDataIndex], 0.0f, jmax(maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f); // map data to produce intensity

        spectImage.setPixelAt(lastRow, i, juce::Colour::fromHSV(intensity, 1.0f, intensity, 1.0f)); // set pixel based on level value
    }
}

void Spectrogram::drawSpectrum()
{
    spectrumCurve.clear();
    float mindB = -100.0f;
    float maxdB = 0.0f;

    for (int i = 0; i < scopeSize; i++) // [3]
    {
        double skewedProportionX = mapToLog10(double(i) / double(scopeSize), 0.1, (double)fftSize / 2); // skew placement to better represent frequencies, more spread data, fx. 0-4000Hz takes more space
        int fftDataIndex = jlimit(0, fftSize / 2, roundToInt(skewedProportionX));                       // Limit the skewed X to fit in the array so we can get it from

        //  float skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.5f);
        // int fftDataIndex = jlimit(0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f));

        float level = jmap(jlimit(mindB, maxdB, Decibels::gainToDecibels(fftWindowDataOut[fftDataIndex]) - juce::Decibels::gainToDecibels((float)fftSize)),
                           mindB,
                           maxdB,
                           0.0f,
                           1.0f);

        scopeData[i] = jmap(level, 0.0f, 1.0f, (float)(height - (height / 10)), 0.0f);
    }

    spectrumCurve.startNewSubPath(0, (float)scopeData[0]);

    for (int i = 1; i < scopeSize; i++)
    {
        if (scopeData[i] != scopeData[i - 1] && i < scopeSize - 50)
        {
            spectrumCurve.lineTo({(float)jmap(i, 0, scopeSize - 1, 0, width),
                                  (float)scopeData[i]});
        }
        else if (i > scopeSize - 50)
        {
            spectrumCurve.lineTo({(float)jmap(i, 0, scopeSize - 1, 0, width),
                                  (float)scopeData[i]});
        }
    }
}

void Spectrogram::timerCallback()
{
    if (nextFFTBlockReady)
    {
        drawSpectrogram();
        drawSpectrum();
        nextFFTBlockReady = false;
        repaint();
    }
}