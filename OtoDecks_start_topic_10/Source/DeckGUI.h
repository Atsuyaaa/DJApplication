/*
  ==============================================================================
    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This class represents the graphical user interface (GUI) for controlling a deck.
    It handles the playback, volume, speed, position, waveform display, loop controls,
    file loading, and mouse hover effects.
    (PERSONAL CONTRIBUTION: Added looping, zoom, drag-and-drop functionality, and mouse hover effects)
*/
class DeckGUI : public Component,
                public Button::Listener, 
                public Slider::Listener, 
                public FileDragAndDropTarget, 
                public Timer
{
public:
    /**
     * Constructor for DeckGUI.
     * @param player Pointer to DJAudioPlayer for controlling playback.
     * @param formatManagerToUse Reference to AudioFormatManager for handling audio formats.
     * @param cacheToUse Reference to AudioThumbnailCache for caching waveform thumbnails.
     * @param _playlistComponent Pointer to PlaylistComponent for track management.
     */
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager & formatManagerToUse,
           AudioThumbnailCache & cacheToUse,
           PlaylistComponent* _playlistComponent);

    /** Destructor */
    ~DeckGUI() override;

    /**
     * Paint method to draw the DeckGUI component.
     * @param g The graphics context used for drawing.
     */
    void paint(Graphics&) override;

    /**
     * Resizes the component and its child components (buttons, sliders, labels).
     */
    void resized() override;

    /**
     * Handles button click events (play, stop, load, and loop buttons).
     * @param button The button that was clicked.
     */
    void buttonClicked(Button*) override;

    /**
     * Handles slider value changes (volume, speed, position, and zoom sliders).
     * @param slider The slider whose value changed.
     */
    void sliderValueChanged(Slider* slider) override;

    /**
     * Checks whether the DeckGUI is interested in a file drag event.
     * @param files The files being dragged.
     * @return True if the files are audio files (e.g., .mp3), false otherwise.
     */
    bool isInterestedInFileDrag(const StringArray& files) override;

    /**
     * Handles files being dropped onto the DeckGUI.
     * @param files The files that were dropped.
     * @param x The x-coordinate where the files were dropped.
     * @param y The y-coordinate where the files were dropped.
     */
    void filesDropped(const StringArray& files, int x, int y) override;

    /**
     * Loads a track into the deck from a drag-and-drop event.
     * (PERSONAL CONTRIBUTION)
     * @param filePath The path of the file to load.
     */
    void loadTrackFromDrag(const String& filePath);

    /**
     * Timer callback that is called periodically to update the waveform display
     * and track title, and handle looping logic.
     * (PERSONAL CONTRIBUTION: Loop functionality and track title updating)
     */
    void timerCallback() override;

    /**
     * Handles mouse entering the button area, providing hover feedback.
     * (PERSONAL CONTRIBUTION)
     * @param event The mouse event information.
     */
    void mouseEnter(const MouseEvent& event) override;

    /**
     * Handles mouse exiting the button area, reverting hover feedback.
     * (PERSONAL CONTRIBUTION)
     * @param event The mouse event information.
     */
    void mouseExit(const MouseEvent& event) override;

private:
    /** File chooser for loading audio files */
    juce::FileChooser fChooser{"Select a file..."};

    /** Play, stop, and load buttons */
    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};
  
    /** Volume, speed, and position sliders */
    Slider volSlider; 
    Slider speedSlider;
    Slider posSlider;

    /** Labels for sliders (PERSONAL CONTRIBUTION: Added labels for sliders) */
    Label volumeLabel;
    Label speedLabel;
    Label positionLabel;

    /** Waveform display to visualize the loaded track */
    WaveformDisplay waveformDisplay;

    /** Pointer to the DJAudioPlayer for controlling playback */
    DJAudioPlayer* player; 

    /** Colors for button hover effects */
    Colour defaultButtonColor = Colours::lightgrey;
    Colour hoverButtonColor = Colours::orange;

    /** Loop control buttons (PERSONAL CONTRIBUTION: Added loop buttons and logic) */
    TextButton setLoopStartButton{"Set Loop Start"};
    TextButton setLoopEndButton{"Set Loop End"};
    TextButton toggleLoopButton{"Loop On/Off"};

    /** Loop start and end points (PERSONAL CONTRIBUTION: Added loop variables) */
    double loopStart = 0.0;
    double loopEnd = 0.0;

    /** Zoom slider for controlling waveform zoom (PERSONAL CONTRIBUTION) */
    Slider zoomSlider;

    /** Label for displaying the track title (PERSONAL CONTRIBUTION) */
    Label trackTitleLabel;

    /** Pointer to the PlaylistComponent for managing tracks */
    PlaylistComponent* playlistComponent;

    /** JUCE macro to prevent copying or assigning the DeckGUI object */
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
