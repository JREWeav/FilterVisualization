#include "AudioGUI.h"

//==============================================================================
AudioGUI::AudioGUI()
{
    audForManager.registerBasicFormats();

    // Buttons
    addAndMakeVisible(loadBtn);
    addAndMakeVisible(playBtn);

    loadBtn.addListener(this);
    playBtn.addListener(this);

    // Gain Slider
    addAndMakeVisible(gainSlider);

    gainSlider.setRange(0, 2);
    gainSlider.setValue(1);
    gainSlider.setNumDecimalPlacesToDisplay(2);
    gainSlider.addListener(this);

    // Filter controls
    addAndMakeVisible(lowPassFreq);
    addAndMakeVisible(lowPassSlope);
    addAndMakeVisible(peakFreq);
    addAndMakeVisible(peakGain);
    addAndMakeVisible(peakQ);
    addAndMakeVisible(highPassFreq);
    addAndMakeVisible(highPassSlope);

    lowPassFreq.setRange(20.0, 20000.0, 1);
    lowPassFreq.setValue(20000);
    lowPassFreq.setNumDecimalPlacesToDisplay(0);
    lowPassFreq.addListener(this);

    lowPassSlope.setRange(1, 4, 1);
    lowPassSlope.setValue(1);
    lowPassSlope.setNumDecimalPlacesToDisplay(0);
    lowPassSlope.addListener(this);

    highPassFreq.setRange(20.0, 20000.0, 1);
    highPassFreq.setValue(20);
    highPassFreq.setNumDecimalPlacesToDisplay(0);
    highPassFreq.addListener(this);

    highPassSlope.setRange(1, 4, 1);
    highPassSlope.setValue(1);
    highPassSlope.setNumDecimalPlacesToDisplay(0);
    highPassSlope.addListener(this);

    peakFreq.setRange(20.0, 20000.0, 1);
    peakFreq.setValue(1000);
    peakFreq.setNumDecimalPlacesToDisplay(0);
    peakFreq.addListener(this);

    peakGain.setRange(-24.0, 24.0, 1);
    peakGain.setValue(0);
    peakGain.setNumDecimalPlacesToDisplay(0);
    peakGain.addListener(this);

    peakQ.setRange(0.0, 1.0, 0.05);
    peakQ.setValue(0.5);
    peakQ.setNumDecimalPlacesToDisplay(2);
    peakQ.addListener(this);

    // Spectrogram
    addAndMakeVisible(spectImage);

    isPlaying = false;

    lowPassFreqStored = 5000.0f;

    addAndMakeVisible(filterLine);

    // Labels
    addAndMakeVisible(peakFreqLab);
    addAndMakeVisible(peakGainLab);
    addAndMakeVisible(peakQLab);
    addAndMakeVisible(lpFreqLab);
    addAndMakeVisible(lpSlopeLab);
    addAndMakeVisible(hpFreqLab);
    addAndMakeVisible(hpSlopeLab);

    peakFreqLab.setText("Peak Frequency", juce::NotificationType::dontSendNotification);
    peakGainLab.setText("Peak Gain", juce::NotificationType::dontSendNotification);
    peakQLab.setText("Peak Q", juce::NotificationType::dontSendNotification);

    lpFreqLab.setText("Low Cut Frequency", juce::NotificationType::dontSendNotification);
    lpSlopeLab.setText("Low Cut Slope", juce::NotificationType::dontSendNotification);

    hpFreqLab.setText("High Cut Frequency", juce::NotificationType::dontSendNotification);
    hpSlopeLab.setText("High Cut Slope", juce::NotificationType::dontSendNotification);

    peakFreqLab.setInterceptsMouseClicks(false, false);
    peakGainLab.setInterceptsMouseClicks(false, false);
    peakQLab.setInterceptsMouseClicks(false, false);

    lpFreqLab.setInterceptsMouseClicks(false, false);
    lpSlopeLab.setInterceptsMouseClicks(false, false);

    hpFreqLab.setInterceptsMouseClicks(false, false);
    hpSlopeLab.setInterceptsMouseClicks(false, false);
}

// Audio

void AudioGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audPlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    filter.prepareToPlay(samplesPerBlockExpected, sampleRate);
    spectImage.prepareToPlay(samplesPerBlockExpected, sampleRate);
    sampleRateStored = sampleRate;
}

void AudioGUI::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    if (audPlayer.isPlaying() != true)
        return;

    audPlayer.getNextAudioBlock(bufferToFill);
    filter.getNextAudioBlock(bufferToFill);
    spectImage.getNextAudioBlock(bufferToFill);
}
void AudioGUI::releaseResources()
{
    audPlayer.releaseResources();
    filter.releaseResources();
    spectImage.releaseResources();
}

// Graphics/GUI

void AudioGUI::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::purple);
    g.setFont(juce::Font(16.0f));
}

void AudioGUI::resized()
{
    float colW = getWidth() / 6;
    float rowH = getHeight() / 8;

    loadBtn.setBounds(0, 0, colW * 3, rowH);
    playBtn.setBounds(colW * 3, 0, colW * 3, rowH);

    gainSlider.setBounds(0, rowH, getWidth(), rowH);

    // Draw visuals
    spectImage.setBounds(0, rowH * 2, getWidth(), rowH * 3);
    filterLine.setBounds(0, rowH * 2, getWidth(), rowH * 3);

    // Filter Controls
    highPassFreq.setBounds(0, rowH * 5, colW * 2, rowH);
    highPassSlope.setBounds(0, rowH * 6, colW * 2, rowH);

    peakFreq.setBounds(colW * 2, rowH * 5, colW * 2, rowH);
    peakGain.setBounds(colW * 2, rowH * 6, colW * 2, rowH);
    peakQ.setBounds(colW * 2, rowH * 7, colW * 2, rowH);

    lowPassFreq.setBounds(colW * 4, rowH * 5, colW * 2, rowH);
    lowPassSlope.setBounds(colW * 4, rowH * 6, colW * 2, rowH);

    // Labels
    hpFreqLab.setBounds(0, rowH * 5 - getHeight() / 25, colW * 2, rowH);
    hpSlopeLab.setBounds(0, rowH * 6 - getHeight() / 25, colW * 2, rowH);

    peakFreqLab.setBounds(colW * 2, rowH * 5 - getHeight() / 25, colW * 2, rowH);
    peakGainLab.setBounds(colW * 2, rowH * 6 - getHeight() / 25, colW * 2, rowH);
    peakQLab.setBounds(colW * 2, rowH * 7 - getHeight() / 25, colW * 2, rowH);

    lpFreqLab.setBounds(colW * 4, rowH * 5 - getHeight() / 25, colW * 2, rowH);
    lpSlopeLab.setBounds(colW * 4, rowH * 6 - getHeight() / 25, colW * 2, rowH);
}

void AudioGUI::buttonClicked(Button *btn)
{
    if (btn == &loadBtn)
    {
        if (fileChooser.browseForFileToOpen())
        {
            playBtn.setButtonText("STOPPED");
            isPlaying = false;
            audPlayer.pause();
            if (audForManager.createReaderFor(fileChooser.getResult()) != nullptr)
            {
                loadBtn.setButtonText("FILE LOADED");
                audPlayer.loadFile(fileChooser.getResult());
            }
            else
            {
                loadBtn.setButtonText("LOAD FAILED");
            }
        }
    }
    if (btn == &playBtn)
    {
        if (isPlaying)
        {
            playBtn.setButtonText("STOPPED");
            audPlayer.pause();
            isPlaying = false;
        }
        else
        {
            playBtn.setButtonText("PLAYING");
            audPlayer.play();
            isPlaying = true;
        }
    }
}

void AudioGUI::sliderValueChanged(Slider *sldr)
{
    if (sldr == &gainSlider)
    {
        audPlayer.setGain(sldr->getValue());
    }
    // Filter Controllers
    // Low Pass
    if (sldr == &lowPassFreq)
    {
        filter.setLowPassFreq((float)sldr->getValue());
    }
    if (sldr == &lowPassSlope)
    {
        filter.setLowPassSlope(sldr->getValue());
    }
    // High Pass
    if (sldr == &highPassFreq)
    {
        filter.setHighPassFreq((float)sldr->getValue());
    }
    if (sldr == &highPassSlope)
    {
        filter.setHighPassSlope(sldr->getValue());
    }
    // Peak
    if (sldr == &peakFreq)
    {
        filter.setPeakFreq((float)sldr->getValue());
    }
    if (sldr == &peakGain)
    {
        filter.setPeakGain((float)sldr->getValue());
    }
    if (sldr == &peakQ)
    {
        filter.setPeakQ((float)sldr->getValue());
    }
}
