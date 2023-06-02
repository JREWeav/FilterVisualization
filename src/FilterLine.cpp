#include "FilterLine.h"

//==============================================================================
FilterLine::FilterLine(AudioFilter *_filter) : filter(_filter)
{
    startTimerHz(20);
}

// Graphics/GUI

void FilterLine::paint(juce::Graphics &g)
{
    g.setColour(juce::Colours::white);
    g.strokePath(responseDisplay, PathStrokeType(2.f));
}

void FilterLine::resized()
{
    width = getWidth();
    height = getHeight();
}

void FilterLine::calcLinePoints()
{
    responseDisplay.clear();
    points.resize(width);

    for (auto i = 0; i < points.size(); i++)
    {
        double pointPlacement = 1.0f;

        // double skewedProportionX = 1.0 - std::exp(std::log(1.0f - (float)i / (float)width) * 0.5f);
        // double freq = skewedProportionX * 20000.0;

        double freq = mapToLog10(double(i) / double(width), 20.0, 20000.0);

        // Peak
        if (!filter->leftPath.isBypassed<1>())
        {
            pointPlacement *= filter->leftPath.get<1>().coefficients->getMagnitudeForFrequency(freq, filter->getSampleRate());
        }

        // Lowcut
        if (!filter->leftPath.get<0>().isBypassed<0>())
        {
            pointPlacement *= filter->leftPath.get<0>().get<0>().coefficients->getMagnitudeForFrequency(freq, filter->getSampleRate());
        }
        if (!filter->leftPath.get<0>().isBypassed<1>())
        {
            pointPlacement *= filter->leftPath.get<0>().get<1>().coefficients->getMagnitudeForFrequency(freq, filter->getSampleRate());
        }
        if (!filter->leftPath.get<0>().isBypassed<2>())
        {
            pointPlacement *= filter->leftPath.get<0>().get<2>().coefficients->getMagnitudeForFrequency(freq, filter->getSampleRate());
        }
        if (!filter->leftPath.get<0>().isBypassed<3>())
        {
            pointPlacement *= filter->leftPath.get<0>().get<3>().coefficients->getMagnitudeForFrequency(freq, filter->getSampleRate());
        }

        // Highcut
        if (!filter->leftPath.get<2>().isBypassed<0>())
        {
            pointPlacement *= filter->leftPath.get<2>().get<0>().coefficients->getMagnitudeForFrequency(freq, filter->getSampleRate());
        }
        if (!filter->leftPath.get<2>().isBypassed<1>())
        {
            pointPlacement *= filter->leftPath.get<2>().get<1>().coefficients->getMagnitudeForFrequency(freq, filter->getSampleRate());
        }
        if (!filter->leftPath.get<2>().isBypassed<2>())
        {
            pointPlacement *= filter->leftPath.get<2>().get<2>().coefficients->getMagnitudeForFrequency(freq, filter->getSampleRate());
        }
        if (!filter->leftPath.get<2>().isBypassed<3>())
        {
            pointPlacement *= filter->leftPath.get<2>().get<3>().coefficients->getMagnitudeForFrequency(freq, filter->getSampleRate());
        }
        points[i] = jmap(Decibels::gainToDecibels(pointPlacement), -24.0, 24.0, double(height) - (height / 10), 0.0);
    }
}

void FilterLine::drawLine()
{
    responseDisplay.startNewSubPath(0, points[0]);

    for (auto i = 1; i < points.size(); i++)
    {
        responseDisplay.lineTo(i, points[i]);
    }
    repaint();
}

void FilterLine::timerCallback()
{
    calcLinePoints();
    drawLine();
}