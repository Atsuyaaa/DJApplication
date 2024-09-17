/*
==============================================================================
    This file was auto-generated!
==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This class is the main component of the application. It manages the three deck GUIs,
    the playlist component, and the overall audio mixing.
    (PERSONAL CONTRIBUTION: Added third deck, theme toggle functionality, and audio mixing).
*/
class MainComponent : public AudioAppComponent
{
public:
    //==============================================================================
    /**
     * Constructor for MainComponent.
     * Initializes the players, deck GUIs, playlist, and sets up the audio format manager.
     * Also adds the theme toggle button and configures the audio mixer for playback.
     */
    MainComponent();

    /** Destructor */
    ~MainComponent() override;

    //==============================================================================
    /**
     * Prepares the audio players for playback by setting up sample rate and buffer size.
     * @param samplesPerBlockExpected The expected number of samples per block.
     * @param sampleRate The audio sample rate.
     */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /**
     * Gets the next block of audio data and passes it to the mixer source.
     * @param bufferToFill The buffer to fill with audio data.
     */
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    /** Releases the audio resources when they are no longer needed. */
    void releaseResources() override;

    //==============================================================================
    /**
     * Paints the main component, filling it with the current theme's background color.
     * @param g The graphics context used for drawing.
     */
    void paint(Graphics& g) override;

    /** Resizes and positions the components (deck GUIs, playlist, and theme toggle button). */
    void resized() override;

    //==============================================================================
    /**
     * Toggles between Light and Dark color themes.
     * (PERSONAL CONTRIBUTION: Implemented theme toggle functionality)
     */
    void toggleTheme();

private:
    //==============================================================================
    // Private member variables for managing the application state and UI components

    /** Enum class to store the current theme type (Light or Dark) */
    enum class Theme { Light, Dark };

    /** The currently selected theme (default is Light) */
    Theme currentTheme = Theme::Light;

    /** Helper method to apply the current theme to the component and its children */
    void applyTheme();

    /** Manages audio formats and decoding (e.g., MP3, WAV) */
    AudioFormatManager formatManager;

    /** Cache for waveform thumbnails to improve performance when visualizing audio tracks */
    AudioThumbnailCache thumbCache{100}; 

    /** PlaylistComponent for managing and displaying the track playlist */
    PlaylistComponent playlistComponent{&player1};  // Pass player1 to PlaylistComponent constructor

    //==============================================================================
    // Audio players and deck GUIs

    /** First DJ audio player */
    DJAudioPlayer player1{formatManager};

    /** First deck GUI (linked to player1) */
    DeckGUI deckGUI1{&player1, formatManager, thumbCache, &playlistComponent};  // Pass PlaylistComponent to DeckGUI

    /** Second DJ audio player */
    DJAudioPlayer player2{formatManager};

    /** Second deck GUI (linked to player2) */
    DeckGUI deckGUI2{&player2, formatManager, thumbCache, &playlistComponent};  // Pass PlaylistComponent to DeckGUI

    /** Third DJ audio player (PERSONAL CONTRIBUTION: Added third player) */
    DJAudioPlayer player3{formatManager};

    /** Third deck GUI (linked to player3) (PERSONAL CONTRIBUTION: Added third deck) */
    DeckGUI deckGUI3{&player3, formatManager, thumbCache, &playlistComponent};  // New GUI component for the third deck

    /** Mixer source for combining audio from all three players */
    MixerAudioSource mixerSource; 

    //==============================================================================
    // UI components

    /** Button to toggle between Light and Dark themes (PERSONAL CONTRIBUTION) */
    TextButton themeToggleButton{"Toggle Theme"}; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
