#include "AudioFilter.h"

//==============================================================================
AudioFilter::AudioFilter()
{
    lowPassFreq = 20000.0f;
    lowPassSlope = 1;
    peakFreq = 1000.0f;
    peakGainAsDB = 0.0f;
    peakQ = 1.0f;
    highPassFreq = 20.0f;
    highPassSlope = 1;
}

// Audio

void AudioFilter::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.sampleRate = sampleRate;
    spec.numChannels = 1;

    leftPath.prepare(spec);
    rightPath.prepare(spec);

    sampleRateStored = sampleRate;

    defineFiltersMakeCoefficients(sampleRate);
}

void AudioFilter::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    defineFiltersMakeCoefficients(sampleRateStored);

    juce::dsp::AudioBlock<float> block(bufferToFill.buffer->getArrayOfWritePointers(),
                                       bufferToFill.buffer->getNumChannels(),
                                       bufferToFill.startSample,
                                       bufferToFill.numSamples);

    juce::dsp::AudioBlock<float> leftBlock(block.getSingleChannelBlock(0));
    juce::dsp::AudioBlock<float> rightBlock(block.getSingleChannelBlock(1));

    leftPath.process(dsp::ProcessContextReplacing<float>(leftBlock));
    rightPath.process(dsp::ProcessContextReplacing<float>(rightBlock));
}

void AudioFilter::releaseResources()
{
}

// Interaction
void AudioFilter::setLowPassFreq(float freq)
{
    lowPassFreq = freq;
}
void AudioFilter::setLowPassSlope(int slope)
{
    lowPassSlope = slope;
}
void AudioFilter::setPeakFreq(float freq)
{
    peakFreq = freq;
}
void AudioFilter::setPeakGain(int gain)
{
    peakGainAsDB = gain;
}
void AudioFilter::setPeakQ(float q)
{
    peakQ = q;
}
void AudioFilter::setHighPassFreq(float freq)
{
    highPassFreq = freq;
}
void AudioFilter::setHighPassSlope(int slope)
{
    highPassSlope = slope;
}

void AudioFilter::defineFiltersMakeCoefficients(double sampleRate)
{
    auto peakCoefficientsLeft = dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, peakFreq, peakQ, Decibels::decibelsToGain(peakGainAsDB));
    auto peakCoefficientsRight = dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, peakFreq, peakQ, Decibels::decibelsToGain(peakGainAsDB));

    *leftPath.get<1>().coefficients = *peakCoefficientsLeft;
    *rightPath.get<1>().coefficients = *peakCoefficientsRight;

    auto lowPassCoefficientsLeft = dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(lowPassFreq, sampleRate, 2 * lowPassSlope);

    leftPath.get<0>().setBypassed<0>(true);
    leftPath.get<0>().setBypassed<1>(true);
    leftPath.get<0>().setBypassed<2>(true);
    leftPath.get<0>().setBypassed<3>(true);

    if (lowPassSlope == 1)
    {
        leftPath.get<0>().get<0>().coefficients = lowPassCoefficientsLeft[0];
        leftPath.get<0>().setBypassed<0>(false);
    }
    else if (lowPassSlope == 2)
    {
        leftPath.get<0>().get<0>().coefficients = lowPassCoefficientsLeft[0];
        leftPath.get<0>().setBypassed<0>(false);
        leftPath.get<0>().get<1>().coefficients = lowPassCoefficientsLeft[1];
        leftPath.get<0>().setBypassed<1>(false);
    }
    else if (lowPassSlope == 3)
    {
        leftPath.get<0>().get<0>().coefficients = lowPassCoefficientsLeft[0];
        leftPath.get<0>().setBypassed<0>(false);
        leftPath.get<0>().get<1>().coefficients = lowPassCoefficientsLeft[1];
        leftPath.get<0>().setBypassed<1>(false);
        leftPath.get<0>().get<2>().coefficients = lowPassCoefficientsLeft[2];
        leftPath.get<0>().setBypassed<2>(false);
    }
    else if (lowPassSlope == 4)
    {
        leftPath.get<0>().get<0>().coefficients = lowPassCoefficientsLeft[0];
        leftPath.get<0>().setBypassed<0>(false);
        leftPath.get<0>().get<1>().coefficients = lowPassCoefficientsLeft[1];
        leftPath.get<0>().setBypassed<1>(false);
        leftPath.get<0>().get<2>().coefficients = lowPassCoefficientsLeft[2];
        leftPath.get<0>().setBypassed<2>(false);
        leftPath.get<0>().get<3>().coefficients = lowPassCoefficientsLeft[3];
        leftPath.get<0>().setBypassed<3>(false);
    }

    auto lowPassCoefficientsRight = dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(lowPassFreq, sampleRate, 2 * lowPassSlope);

    rightPath.get<0>().setBypassed<0>(true);
    rightPath.get<0>().setBypassed<1>(true);
    rightPath.get<0>().setBypassed<2>(true);
    rightPath.get<0>().setBypassed<3>(true);

    if (lowPassSlope == 1)
    {
        rightPath.get<0>().get<0>().coefficients = lowPassCoefficientsRight[0];
        rightPath.get<0>().setBypassed<0>(false);
    }
    else if (lowPassSlope == 2)
    {
        rightPath.get<0>().get<0>().coefficients = lowPassCoefficientsRight[0];
        rightPath.get<0>().setBypassed<0>(false);
        rightPath.get<0>().get<1>().coefficients = lowPassCoefficientsRight[1];
        rightPath.get<0>().setBypassed<1>(false);
    }
    else if (lowPassSlope == 3)
    {
        rightPath.get<0>().get<0>().coefficients = lowPassCoefficientsRight[0];
        rightPath.get<0>().setBypassed<0>(false);
        rightPath.get<0>().get<1>().coefficients = lowPassCoefficientsRight[1];
        rightPath.get<0>().setBypassed<1>(false);
        rightPath.get<0>().get<2>().coefficients = lowPassCoefficientsRight[2];
        rightPath.get<0>().setBypassed<2>(false);
    }
    else if (lowPassSlope == 4)
    {
        rightPath.get<0>().get<0>().coefficients = lowPassCoefficientsRight[0];
        rightPath.get<0>().setBypassed<0>(false);
        rightPath.get<0>().get<1>().coefficients = lowPassCoefficientsRight[1];
        rightPath.get<0>().setBypassed<1>(false);
        rightPath.get<0>().get<2>().coefficients = lowPassCoefficientsRight[2];
        rightPath.get<0>().setBypassed<2>(false);
        rightPath.get<0>().get<3>().coefficients = lowPassCoefficientsRight[3];
        rightPath.get<0>().setBypassed<3>(false);
    }

    auto highPassCoefficientsLeft = dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(highPassFreq, sampleRate, 2 * highPassSlope);

    leftPath.get<2>().setBypassed<0>(true);
    leftPath.get<2>().setBypassed<1>(true);
    leftPath.get<2>().setBypassed<2>(true);
    leftPath.get<2>().setBypassed<3>(true);

    if (highPassSlope == 1)
    {
        leftPath.get<2>().get<0>().coefficients = highPassCoefficientsLeft[0];
        leftPath.get<2>().setBypassed<0>(false);
    }
    else if (highPassSlope == 2)
    {
        leftPath.get<2>().get<0>().coefficients = highPassCoefficientsLeft[0];
        leftPath.get<2>().setBypassed<0>(false);
        leftPath.get<2>().get<1>().coefficients = highPassCoefficientsLeft[1];
        leftPath.get<2>().setBypassed<1>(false);
    }
    else if (highPassSlope == 3)
    {
        leftPath.get<2>().get<0>().coefficients = highPassCoefficientsLeft[0];
        leftPath.get<2>().setBypassed<0>(false);
        leftPath.get<2>().get<1>().coefficients = highPassCoefficientsLeft[1];
        leftPath.get<2>().setBypassed<1>(false);
        leftPath.get<2>().get<2>().coefficients = highPassCoefficientsLeft[2];
        leftPath.get<2>().setBypassed<2>(false);
    }
    else if (highPassSlope == 4)
    {
        leftPath.get<2>().get<0>().coefficients = highPassCoefficientsLeft[0];
        leftPath.get<2>().setBypassed<0>(false);
        leftPath.get<2>().get<1>().coefficients = highPassCoefficientsLeft[1];
        leftPath.get<2>().setBypassed<1>(false);
        leftPath.get<2>().get<2>().coefficients = highPassCoefficientsLeft[2];
        leftPath.get<2>().setBypassed<2>(false);
        leftPath.get<2>().get<3>().coefficients = highPassCoefficientsLeft[3];
        leftPath.get<2>().setBypassed<3>(false);
    }

    auto highPassCoefficientsRight = dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(highPassFreq, sampleRate, 2 * highPassSlope);

    rightPath.get<2>().setBypassed<0>(true);
    rightPath.get<2>().setBypassed<1>(true);
    rightPath.get<2>().setBypassed<2>(true);
    rightPath.get<2>().setBypassed<3>(true);

    if (highPassSlope == 1)
    {
        rightPath.get<2>().get<0>().coefficients = highPassCoefficientsRight[0];
        rightPath.get<2>().setBypassed<0>(false);
    }
    else if (highPassSlope == 2)
    {
        rightPath.get<2>().get<0>().coefficients = highPassCoefficientsRight[0];
        rightPath.get<2>().setBypassed<0>(false);
        rightPath.get<2>().get<1>().coefficients = highPassCoefficientsRight[1];
        rightPath.get<2>().setBypassed<1>(false);
    }
    else if (highPassSlope == 3)
    {
        rightPath.get<2>().get<0>().coefficients = highPassCoefficientsRight[0];
        rightPath.get<2>().setBypassed<0>(false);
        rightPath.get<2>().get<1>().coefficients = highPassCoefficientsRight[1];
        rightPath.get<2>().setBypassed<1>(false);
        rightPath.get<2>().get<2>().coefficients = highPassCoefficientsRight[2];
        rightPath.get<2>().setBypassed<2>(false);
    }
    else if (highPassSlope == 4)
    {
        rightPath.get<2>().get<0>().coefficients = highPassCoefficientsRight[0];
        rightPath.get<2>().setBypassed<0>(false);
        rightPath.get<2>().get<1>().coefficients = highPassCoefficientsRight[1];
        rightPath.get<2>().setBypassed<1>(false);
        rightPath.get<2>().get<2>().coefficients = highPassCoefficientsRight[2];
        rightPath.get<2>().setBypassed<2>(false);
        rightPath.get<2>().get<3>().coefficients = highPassCoefficientsRight[3];
        rightPath.get<2>().setBypassed<3>(false);
    }
}

double AudioFilter::getSampleRate()
{
    return sampleRateStored;
}
