#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <JuceHeader.h>
#include "AudioFilter.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class FilterLine : public juce::Component, public juce::Timer
{
public:
    //==============================================================================
    FilterLine(AudioFilter *_filter);

    // Graphics/GUI
    void paint(juce::Graphics &) override;
    void resized() override;

    void sampleIntoArray(float sample);

    // Drawing
    void calcLinePoints();
    void drawLine();

    // Timer
    // void changeListenerCallback(ChangeBroadcaster *source) override;

    void timerCallback() override;

private:
    //==============================================================================
    // Your private member variables go here...
    std::vector<double> points;
    std::vector<double> pointsCopy;
    AudioFilter *filter;
    Path responseDisplay;
    int width;
    int height;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterLine)
};
