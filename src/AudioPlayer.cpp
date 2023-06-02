#include "AudioPlayer.h"

//==============================================================================
AudioPlayer::AudioPlayer(AudioFormatManager *afm) : audioForMan(afm)
{
}

// Audio

void AudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audTrans.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
    }
    else
    {
        audTrans.getNextAudioBlock(bufferToFill);
    }
}

void AudioPlayer::releaseResources()
{
    audTrans.releaseResources();
}

void AudioPlayer::loadFile(File file)
{
    audTrans.stop();
    auto *reader = audioForMan->createReaderFor(file);
    auto tempSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
    audTrans.setSource(tempSource.get(), 0, nullptr, reader->sampleRate);
    readerSource.reset(tempSource.release());
}

void AudioPlayer::play()
{
    audTrans.start();
}

void AudioPlayer::pause()
{
    audTrans.stop();
}

void AudioPlayer::setGain(float gain)
{
    audTrans.setGain(gain);
}

bool AudioPlayer::isPlaying()
{
    return audTrans.isPlaying();
}