/*
==============================================================================
    WaveformDisplay.cpp
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew
==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
// Constructor: Initializes the audio thumbnail and adds a change listener to respond
// when the waveform data is updated. The zoom level is initialized to 1.0 (no zoom).
// (PERSONAL CONTRIBUTION: Added zoom functionality)
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
                                 AudioThumbnailCache& cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0),
                                 zoomLevel(1.0)  // Initialize zoom level
{
    audioThumb.addChangeListener(this);  // Listen for changes in the audio thumbnail (e.g., when it finishes loading)
}

WaveformDisplay::~WaveformDisplay()
{
}

//==============================================================================
// Paint method: Draws the waveform and handles zooming and playhead position.
// (PERSONAL CONTRIBUTION: Added zoom and playhead visualization)
void WaveformDisplay::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));  // Clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);  // Draw an outline around the component

    if (fileLoaded)
    {
        g.setColour(Colours::orange);

        // Calculate the zoomed-in portion of the waveform based on the zoom level
        double audioLength = audioThumb.getTotalLength();
        double startTime = visibleStart * audioLength;
        double endTime = visibleEnd * audioLength;

        // Scale the waveform to fit the zoom level
        Rectangle<int> bounds = getLocalBounds();
        bounds.setHeight(bounds.getHeight() * verticalZoom);  // Apply vertical zoom

        // Draw the waveform with zoom applied
        audioThumb.drawChannel(g, bounds, startTime, endTime, 0, verticalZoom);

        // Draw the current playhead position
        g.setColour(Colours::lightgreen);
        g.drawRect((position - visibleStart) * getWidth() / (visibleEnd - visibleStart), 0, 2, getHeight());
    }
    else
    {
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(), Justification::centred, true);  // Display text if no file is loaded
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains.
}

//==============================================================================
// Loads an audio file from a URL into the waveform display.
void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear();  // Clear any previously loaded audio
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));  // Load the audio file into the thumbnail
    if (fileLoaded)
    {
        std::cout << "wfd: loaded! " << std::endl;
        repaint();  // Redraw the waveform once the file is loaded
    }
    else
    {
        std::cout << "wfd: not loaded! " << std::endl;
    }
}

//==============================================================================
// Responds to changes in the audio thumbnail (e.g., when the waveform data changes).
void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    std::cout << "wfd: change received! " << std::endl;
    repaint();  // Redraw the waveform when a change is detected
}

//==============================================================================
// Updates the current playhead position and adjusts the visible area based on the zoom level.
// (PERSONAL CONTRIBUTION: Added playhead and zoom adjustment logic)
void WaveformDisplay::setPositionRelative(double pos)
{
    position = pos;

    // Adjust the visible area to follow the playhead within the zoom level
    if (zoomLevel > 1.0)
    {
        // Center the visible area around the current position
        visibleStart = position - (1.0 / zoomLevel) / 2;
        visibleEnd = position + (1.0 / zoomLevel) / 2;

        // Ensure the visible range is within bounds
        if (visibleStart < 0.0)
            visibleStart = 0.0;
        if (visibleEnd > 1.0)
            visibleEnd = 1.0;
    }

    repaint();  // Redraw the waveform to reflect the new position
}

//==============================================================================
// Sets the zoom level for the waveform display.
// (PERSONAL CONTRIBUTION: Added zoom functionality for better waveform navigation)
void WaveformDisplay::setZoomLevel(double zoom)
{
    zoomLevel = zoom;

    // Adjust the visible start and end points based on the zoom level
    visibleStart = position - (1.0 / zoomLevel) / 2;
    visibleEnd = position + (1.0 / zoomLevel) / 2;

    // Ensure the visible range is within bounds
    if (visibleStart < 0.0)
        visibleStart = 0.0;
    if (visibleEnd > 1.0)
        visibleEnd = 1.0;

    repaint();  // Redraw the waveform with the updated zoom level
}

//==============================================================================
// Sets the vertical zoom level to scale the height of the waveform.
// (PERSONAL CONTRIBUTION: Added vertical zoom functionality)
void WaveformDisplay::setVerticalZoom(double zoom)
{
    verticalZoom = zoom;
    repaint();  // Redraw the waveform with the updated vertical zoom level
}
