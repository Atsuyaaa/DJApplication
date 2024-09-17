/*
==============================================================================
DJAudioPlayer.h
Created: 13 Mar 2020 4:22:22pm
Author:  matthew
==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
// DJAudioPlayer class manages audio playback, including gain, speed, position, 
// and looping functionality. It uses JUCE's AudioSource for managing audio streams.
// (PERSONAL CONTRIBUTION: Looping functionality, track title management)
class DJAudioPlayer : public AudioSource 
{
public:
    /**
     * Constructor for DJAudioPlayer.
     * @param _formatManager Reference to AudioFormatManager for handling audio formats.
     */
    DJAudioPlayer(AudioFormatManager& _formatManager);

    /** Destructor */
    ~DJAudioPlayer() override;

    /**
     * Prepares the audio player to play, initializing resources based on the given 
     * sample rate and block size.
     * @param samplesPerBlockExpected Number of samples per block expected.
     * @param sampleRate The audio sample rate.
     */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /**
     * Fills the next block of audio data, overriding JUCE's AudioSource method.
     * Implements looping if enabled.
     * (PERSONAL CONTRIBUTION: Overriding method for handling looping)
     * @param bufferToFill The audio buffer to fill with audio data.
     */
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    /** Releases audio resources when no longer needed. */
    void releaseResources() override;

    //==============================================================================
    // Looping functionality (PERSONAL CONTRIBUTION)
    
    /**
     * Sets the start point for the loop.
     * @param start The start position of the loop in seconds.
     */
    void setLoopStart(double start);

    /**
     * Sets the end point for the loop.
     * @param end The end position of the loop in seconds.
     */
    void setLoopEnd(double end);

    /**
     * Enables or disables the loop.
     * @param shouldLoop True if looping should be enabled, false otherwise.
     */
    void enableLoop(bool shouldLoop);

    /**
     * Checks if looping is currently enabled.
     * @return True if looping is enabled, false otherwise.
     */
    bool getIsLooping() const;

    //==============================================================================
    
    /**
     * Loads an audio file from a URL and prepares it for playback.
     * @param audioURL The URL of the audio file to load.
     */
    void loadURL(URL audioURL);

    /**
     * Sets the playback gain (volume).
     * @param gain The gain value between 0.0 (mute) and 1.0 (full volume).
     */
    void setGain(double gain);

    /**
     * Sets the playback speed.
     * @param ratio The speed ratio where 1.0 is normal speed, values greater than 1 speed up playback.
     */
    void setSpeed(double ratio);

    /**
     * Sets the playback position in seconds.
     * @param posInSecs The playback position in seconds.
     */
    void setPosition(double posInSecs);

    /**
     * Sets the playback position relative to the track's length.
     * @param pos A value between 0.0 and 1.0 representing the relative position.
     */
    void setPositionRelative(double pos);

    /** Starts audio playback. */
    void start();

    /** Stops audio playback. */
    void stop();

    /**
     * Gets the relative position of the playhead in the track.
     * @return A value between 0.0 and 1.0 representing the relative position of the playhead.
     */
    double getPositionRelative();

    /**
     * Gets the title of the currently loaded track.
     * (PERSONAL CONTRIBUTION: Track title handling)
     * @return A string containing the track title.
     */
    String getTrackTitle() const;

private:
    /** Reference to the AudioFormatManager used to handle audio formats */
    AudioFormatManager& formatManager;

    /** Unique pointer to an AudioFormatReaderSource for managing audio reading */
    std::unique_ptr<AudioFormatReaderSource> readerSource;

    /** AudioTransportSource for managing playback control (play, stop, seek) */
    AudioTransportSource transportSource;

    /** ResamplingAudioSource for adjusting playback speed */
    ResamplingAudioSource resampleSource{&transportSource, false, 2};

    // Looping-related variables (PERSONAL CONTRIBUTION)
    double loopStart = 0.0;  // The start point of the loop in seconds
    double loopEnd = 0.0;    // The end point of the loop in seconds
    bool isLooping = false;  // Boolean flag indicating whether looping is enabled

    /** The title of the currently loaded track (PERSONAL CONTRIBUTION) */
    String trackTitle;
};

