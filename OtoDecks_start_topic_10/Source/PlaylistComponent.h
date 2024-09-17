/*
==============================================================================
    PlaylistComponent.h
    Created: 14 Aug 2024 6:18:46pm
    Author:  Atysuya Ino
==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <string>
#include "DJAudioPlayer.h"

//==============================================================================
/*
    PlaylistComponent is responsible for displaying a list of tracks in a table format.
    Each track in the playlist can be played using the Play button, and new tracks can
    be added dynamically. The component uses a TableListBox to display the tracks.
    (PERSONAL CONTRIBUTION: Added dynamic track addition, Play button functionality)
*/
class PlaylistComponent : public Component,
                          public TableListBoxModel,  // Provides the data and behavior for the table
                          public Button::Listener    // Handles button click events
{
public:
    /**
     * Constructor for PlaylistComponent.
     * @param _player Pointer to the DJAudioPlayer to load and play tracks.
     */
    PlaylistComponent(DJAudioPlayer* _player);

    /** Destructor */
    ~PlaylistComponent() override = default;

    //==============================================================================
    /**
     * Paints the PlaylistComponent, filling the background with the current theme's color.
     * @param g The graphics context used for drawing.
     */
    void paint(juce::Graphics& g) override;

    /** Resizes the PlaylistComponent and its child components. */
    void resized() override;

    //==============================================================================
    /**
     * Returns the number of rows (tracks) in the playlist.
     * @return The number of tracks in the playlist.
     */
    int getNumRows() override;

    /**
     * Paints the background of a row in the table.
     * @param g The graphics context.
     * @param rowNumber The row number to paint.
     * @param width The width of the row.
     * @param height The height of the row.
     * @param rowIsSelected True if the row is selected, false otherwise.
     */
    void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;

    /**
     * Paints the content of a cell in the table.
     * @param g The graphics context.
     * @param rowNumber The row number to paint.
     * @param columnId The ID of the column to paint.
     * @param width The width of the cell.
     * @param height The height of the cell.
     * @param rowIsSelected True if the row is selected, false otherwise.
     */
    void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    /**
     * Refreshes a component in the table (e.g., creating or updating buttons for rows).
     * @param rowNumber The row number to refresh.
     * @param columnId The column ID.
     * @param isRowSelected True if the row is selected, false otherwise.
     * @param existingComponentToUpdate The existing component to update, or nullptr to create a new one.
     * @return The component to display in the cell (e.g., a Play button).
     */
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    /**
     * Handles button click events in the table.
     * Loads and plays the track corresponding to the clicked Play button.
     * @param button The button that was clicked.
     */
    void buttonClicked(juce::Button* button) override;

    /**
     * Adds a new track to the playlist and updates the table to display it.
     * (PERSONAL CONTRIBUTION: Added dynamic track addition)
     * @param trackTitle The title of the track to add.
     */
    void addTrack(const juce::String& trackTitle);

private:
    /** Pointer to the DJAudioPlayer, which is used to load and play tracks */
    DJAudioPlayer* player;  

    /** Vector to store the track titles in the playlist */
    std::vector<juce::String> trackTitles;

    /** TableListBox component to display the track list in a table format */
    TableListBox tableComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
