/*
==============================================================================
    WaveformDisplay.h
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew
==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    The WaveformDisplay class is responsible for rendering a visual representation
    of the audio waveform. It includes functionality for horizontal and vertical zoom,
    as well as the ability to display the playhead position as the track plays.
    (PERSONAL CONTRIBUTION: Added zoom functionality and dynamic playhead positioning)
*/
class WaveformDisplay : public Component, 
                        public ChangeListener
{
public:
    /**
     * Constructor for WaveformDisplay.
     * @param formatManagerToUse Reference to the AudioFormatManager to handle audio formats.
     * @param cacheToUse Reference to the AudioThumbnailCache for caching waveform thumbnails.
     */
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);

    /** Destructor */
    ~WaveformDisplay() override;

    //==============================================================================
    /**
     * Paints the waveform display, including the current playhead position and zoom functionality.
     * @param g The graphics context used for drawing.
     */
    void paint(Graphics& g) override;

    /** Resizes the component and adjusts any child components if necessary. */
    void resized() override;

    //==============================================================================
    /**
     * Called when the waveform data changes (e.g., when the waveform finishes loading).
     * @param source The broadcaster that triggered the change.
     */
    void changeListenerCallback(ChangeBroadcaster* source) override;

    /**
     * Loads an audio file from a URL and prepares it for waveform visualization.
     * @param audioURL The URL of the audio file to load.
     */
    void loadURL(URL audioURL);

    //==============================================================================
    /**
     * Sets the relative position of the playhead within the track.
     * @param pos The relative position (from 0.0 to 1.0).
     */
    void setPositionRelative(double pos);

    //==============================================================================
    /**
     * Sets the horizontal zoom level for the waveform.
     * (PERSONAL CONTRIBUTION: Added zoom functionality for precise waveform navigation)
     * @param zoom The zoom level, where 1.0 represents no zoom and values greater than 1.0 zoom in.
     */
    void setZoomLevel(double zoom);

    /**
     * Sets the vertical zoom level to scale the height of the waveform.
     * (PERSONAL CONTRIBUTION: Added vertical zoom functionality)
     * @param verticalZoom The vertical zoom level.
     */
    void setVerticalZoom(double verticalZoom);

private:
    /** AudioThumbnail object to store and render the waveform */
    AudioThumbnail audioThumb;

    /** Flag to indicate whether a file has been successfully loaded */
    bool fileLoaded;

    /** Current playhead position relative to the track length (from 0.0 to 1.0) */
    double position;

    /** Horizontal zoom level (1.0 means no zoom, greater values zoom in) */
    double zoomLevel = 1.0;

    /** Vertical zoom level (1.0 means normal height, greater values stretch the waveform vertically) */
    double verticalZoom = 1.0;

    /** Variables to control the visible portion of the waveform during zooming */
    double visibleStart = 0.0;  // Start of the visible waveform (as a fraction of the total length)
    double visibleEnd = 1.0;    // End of the visible waveform (as a fraction of the total length)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
