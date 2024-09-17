/*
==============================================================================
DJAudioPlayer.cpp
Created: 13 Mar 2020 4:22:22pm
Author:  matthew
==============================================================================
*/

#include "DJAudioPlayer.h"

//==============================================================================
// Constructor: Initializes the DJAudioPlayer with an AudioFormatManager reference
DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) 
: formatManager(_formatManager)
{

}

DJAudioPlayer::~DJAudioPlayer()
{

}

//==============================================================================
// Prepares the player for audio playback by initializing the transport and resample sources
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

//==============================================================================
// Set the loop start point (PERSONAL CONTRIBUTION: Looping functionality)
void DJAudioPlayer::setLoopStart(double start)
{
    loopStart = start;
}

// Set the loop end point (PERSONAL CONTRIBUTION: Looping functionality)
void DJAudioPlayer::setLoopEnd(double end)
{
    loopEnd = end; 
}

// Enable or disable looping (PERSONAL CONTRIBUTION: Looping functionality)
void DJAudioPlayer::enableLoop(bool shouldLoop)
{
    isLooping = shouldLoop; // Set the looping state
}

// Get the current looping state (PERSONAL CONTRIBUTION: Looping functionality)
bool DJAudioPlayer::getIsLooping() const
{
    return isLooping;
}

//==============================================================================
// Get the next block of audio data, implementing looping logic if enabled
// (PERSONAL CONTRIBUTION: Overriding the default getNextAudioBlock to handle looping)
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource == nullptr)
        return;

    transportSource.getNextAudioBlock(bufferToFill);

    // Handle looping logic
    if (isLooping)
    {
        double currentPosition = transportSource.getCurrentPosition();
        
        // Ensure that loopEnd is greater than loopStart to avoid immediate jumps
        if (currentPosition >= loopEnd && loopEnd > loopStart)
        {
            transportSource.setPosition(loopStart);  // Reset to loop start
        }
    }
}

//==============================================================================
// Release resources held by the transport and resample sources
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

//==============================================================================
// Load an audio file from a URL and update the transport source
void DJAudioPlayer::loadURL(URL audioURL)
{
    transportSource.stop();  // Stop any currently playing audio
    transportSource.setSource(nullptr);  // Reset the source
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr)
    {
        auto newSource = std::make_unique<AudioFormatReaderSource>(reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }

    // (PERSONAL CONTRIBUTION: Store the track title for display)
    trackTitle = audioURL.getFileName();  // Set the track title based on the file name
}

//==============================================================================
// Get the title of the currently loaded track (PERSONAL CONTRIBUTION)
String DJAudioPlayer::getTrackTitle() const
{
    return trackTitle;  // Return the stored track title
}

//==============================================================================
// Set the playback gain (volume)
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
}

//==============================================================================
// Set the playback speed
void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 100.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

//==============================================================================
// Set the playback position in seconds
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

//==============================================================================
// Set the playback position relative to the track's length (from 0 to 1)
void DJAudioPlayer::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

//==============================================================================
// Start audio playback
void DJAudioPlayer::start()
{
    transportSource.start();
}

//==============================================================================
// Stop audio playback
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

//==============================================================================
// Get the current position of the playhead relative to the track's length (from 0 to 1)
double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}
