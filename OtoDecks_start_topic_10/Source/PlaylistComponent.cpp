/*
==============================================================================
    PlaylistComponent.cpp
    Created: 14 Aug 2024 6:18:46pm
    Author:  Atysuya Ino
==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaylistComponent.h"

//==============================================================================
// Constructor: Initializes the PlaylistComponent and populates it with some
// example track titles. Sets up the table header with columns for track titles and buttons.
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player)
    : player(_player)
{
    // Initialize track titles (PERSONAL CONTRIBUTION: Added example track titles)
    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");

    // Set up table columns: 1 for Track Title and 2 for Play button
    tableComponent.getHeader().addColumn("Track Title", 1, 400);
    tableComponent.getHeader().addColumn("", 2, 200);  // Play button column
    tableComponent.setModel(this);  // Set this component as the model for the table

    addAndMakeVisible(tableComponent);  // Make the table visible in the UI
}

//==============================================================================
// Paint method: Fills the background of the PlaylistComponent with the current theme's color.
void PlaylistComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));  // Clear background
}

//==============================================================================
// Returns the number of rows (tracks) in the playlist.
int PlaylistComponent::getNumRows()
{
    return static_cast<int>(trackTitles.size());
}

//==============================================================================
// Resizes the table to fit the component's bounds.
void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

//==============================================================================
// Paints the background of a table row. Highlights the row if it's selected.
void PlaylistComponent::paintRowBackground(juce::Graphics& g, int /*rowNumber*/, int /*width*/, int /*height*/, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);  // Highlight selected row
    else
        g.fillAll(juce::Colours::white);  // Background color for non-selected rows
}

//==============================================================================
// Paints the content of a cell in the table. The first column displays the track title.
void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/)
{
    if (columnId == 1)  // Track Title column
    {
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
}

//==============================================================================
// Creates or updates a cell component (like a button) for specific table cells.
// If the column is the Play button column, create a Play button for each row.
Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate)
{
    if (columnId == 2)  // Play button column
    {
        if (existingComponentToUpdate == nullptr)
        {
            auto btn = std::make_unique<juce::TextButton>("Play");  // Create Play button
            btn->setComponentID(juce::String(rowNumber));  // Set component ID to row number
            btn->addListener(this);  // Add this component as a listener to handle button clicks
            existingComponentToUpdate = btn.release();  // Return the newly created button
        }
    }
    return existingComponentToUpdate;  // Return the existing or new component
}

//==============================================================================
// Handles button click events. Loads and plays the selected track when the Play button is clicked.
void PlaylistComponent::buttonClicked(juce::Button* button)
{
    size_t id = static_cast<size_t>(button->getComponentID().getIntValue());  // Get row ID from button's ComponentID
    if (id >= 0 && id < static_cast<int>(trackTitles.size()))
    {
        // Load the selected track from the current working directory
        juce::File audioFile = juce::File::getCurrentWorkingDirectory().getChildFile(trackTitles[id]);
        if (audioFile.existsAsFile())
        {
            player->loadURL(juce::URL{audioFile});  // Load the file into the DJAudioPlayer
            player->start();  // Start playback
        }
    }
}

//==============================================================================
// Adds a new track to the playlist and updates the table.
// (PERSONAL CONTRIBUTION: Added method to add new tracks dynamically)
void PlaylistComponent::addTrack(const juce::String& trackTitle)
{
    trackTitles.push_back(trackTitle);  // Add new track title to the list
    tableComponent.updateContent();  // Refresh the table to show the new track
}
