/*
  ==============================================================================
    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew
  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
// Constructor: Initializes the Deck GUI, adding sliders, buttons, and waveform display.
// (PERSONAL CONTRIBUTION: Added looping buttons, zoom slider, labels for sliders, and track title label.)
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & formatManagerToUse,
                AudioThumbnailCache & cacheToUse,
                PlaylistComponent* _playlistComponent) 
             : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse),
               playlistComponent(_playlistComponent)   
{

    // Adding play, stop, load buttons to the GUI
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);

    // (PERSONAL CONTRIBUTION: Added buttons for loop functionality)
    addAndMakeVisible(setLoopStartButton);
    addAndMakeVisible(setLoopEndButton);
    addAndMakeVisible(toggleLoopButton);

    // (PERSONAL CONTRIBUTION: Added zoom slider to control waveform zoom level)
    addAndMakeVisible(zoomSlider);  
    zoomSlider.setRange(0.1, 5.0, 0.1);
    zoomSlider.setValue(1.0);
    zoomSlider.addListener(this);

    setLoopStartButton.addListener(this);
    setLoopEndButton.addListener(this);
    toggleLoopButton.addListener(this);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);

    // (PERSONAL CONTRIBUTION: Added labels for sliders and set default text)
    volumeLabel.setText("Volume", dontSendNotification);
    addAndMakeVisible(volumeLabel);
    speedLabel.setText("Speed", dontSendNotification);
    addAndMakeVisible(speedLabel);
    positionLabel.setText("Position", dontSendNotification);
    addAndMakeVisible(positionLabel);

    // (PERSONAL CONTRIBUTION: Track title label for displaying the current track's name)
    trackTitleLabel.setText("Track Title: None", dontSendNotification);
    trackTitleLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(trackTitleLabel);

    startTimer(500); // (PERSONAL CONTRIBUTION: Set a timer to periodically update the waveform and track title)
}

DeckGUI::~DeckGUI()
{
    stopTimer();  // Stop the timer when the DeckGUI is destroyed
}

//==============================================================================
// Paint method: Fills the background and draws an outline around the component.
void DeckGUI::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // Clear the background
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // Draw an outline around the component
}

//==============================================================================
// Resized method: Organizes the layout of buttons, sliders, and labels.
void DeckGUI::resized()
{
    double rowH = getHeight() / 14;

    waveformDisplay.setBounds(0, 0, getWidth(), static_cast<int>(rowH * 8));

    playButton.setBounds(0, static_cast<int>(rowH * 9), getWidth() / 3, static_cast<int>(rowH));
    stopButton.setBounds(getWidth() / 3, static_cast<int>(rowH * 9), getWidth() / 3, static_cast<int>(rowH));
    loadButton.setBounds(2 * getWidth() / 3, static_cast<int>(rowH * 9), getWidth() / 3, static_cast<int>(rowH));

    volSlider.setBounds(0, static_cast<int>(rowH * 11), getWidth() / 3, static_cast<int>(rowH));
    speedSlider.setBounds(getWidth() / 3, static_cast<int>(rowH * 11), getWidth() / 3, static_cast<int>(rowH));
    posSlider.setBounds(2 * getWidth() / 3, static_cast<int>(rowH * 11), getWidth() / 3, static_cast<int>(rowH));

    setLoopStartButton.setBounds(0, static_cast<int>(rowH * 12), getWidth() / 3, static_cast<int>(rowH));
    setLoopEndButton.setBounds(getWidth() / 3, static_cast<int>(rowH * 12), getWidth() / 3, static_cast<int>(rowH));
    toggleLoopButton.setBounds(2 * getWidth() / 3, static_cast<int>(rowH * 12), getWidth() / 3, static_cast<int>(rowH));

    zoomSlider.setBounds(0, static_cast<int>(rowH * 13), getWidth(), static_cast<int>(rowH));

    trackTitleLabel.setBounds(10, 10, getWidth() - 20, 20);  // Position track title label at the top
}

//==============================================================================
// Button click handler: Handles play, stop, load, and loop-related button clicks.
// (PERSONAL CONTRIBUTION: Added loop functionality and button listeners for load and loop controls)
void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        player->start();
    }
    if (button == &stopButton)
    {
        player->stop();
    }
    if (button == &loadButton)
    {
        // File chooser for loading audio files
        fChooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles, 
        [this](const juce::FileChooser& chooser)
        {
            auto file = chooser.getResult();
            if (file.existsAsFile())
            {
                player->loadURL(juce::URL{file});
                waveformDisplay.loadURL(juce::URL{file});
            }
        });
    }
    if (button == &setLoopStartButton)
    {
        loopStart = player->getPositionRelative();  // Set loop start point
    }
    if (button == &setLoopEndButton)
    {
        loopEnd = player->getPositionRelative();  // Set loop end point
    }
    if (button == &toggleLoopButton)
    {
        bool isLooping = !player->getIsLooping();  // Toggle loop state
        player->enableLoop(isLooping);  // Update loop status in player
    }
}

//==============================================================================
// Slider value change handler: Handles volume, speed, position, and zoom slider changes.
// (PERSONAL CONTRIBUTION: Zoom functionality integrated for waveform zoom level)
void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    if (slider == &zoomSlider)
    {
        waveformDisplay.setZoomLevel(slider->getValue());  // Adjust zoom level for waveform display
    }
}

//==============================================================================
// Handles file drag events for loading audio files.
bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    return files.size() == 1 && files[0].endsWith(".mp3");  // Only accept single MP3 file
}

//==============================================================================
// Handles file drop events, triggering track loading.
// (PERSONAL CONTRIBUTION: Load track on drag-and-drop)
void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    if (files.size() == 1)
    {
        loadTrackFromDrag(files[0]);
    }
}

//==============================================================================
// Loads track from drag-and-drop action and updates waveform display.
// (PERSONAL CONTRIBUTION: Added method to load audio from file drag)
void DeckGUI::loadTrackFromDrag(const String& filePath)
{
    File file(filePath);
    if (file.exists())
    {
        URL audioURL(file);
        player->loadURL(audioURL);
        waveformDisplay.loadURL(audioURL);
    }
}

//==============================================================================
// Timer callback: Updates the waveform display and track title periodically.
// (PERSONAL CONTRIBUTION: Loop functionality with automatic track looping)
void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
    trackTitleLabel.setText("Track Title: " + player->getTrackTitle(), dontSendNotification);

    if (player->getIsLooping() && player->getPositionRelative() >= loopEnd)
    {
        player->setPositionRelative(loopStart);  // Loop back to the start
    }
}

//==============================================================================
// Mouse hover effects for buttons.
// (PERSONAL CONTRIBUTION: Added mouse hover effects for buttons)
void DeckGUI::mouseEnter(const MouseEvent& event)
{
    TextButton* button = dynamic_cast<TextButton*>(event.eventComponent);
    if (button)
    {
        button->setColour(TextButton::buttonColourId, Colours::orange);  // Hover color
    }
}

//==============================================================================
// Mouse exit effects for buttons.
// (PERSONAL CONTRIBUTION: Added mouse exit effects for buttons)
void DeckGUI::mouseExit(const MouseEvent& event)
{
    TextButton* button = dynamic_cast<TextButton*>(event.eventComponent);
    if (button)
    {
        button->setColour(TextButton::buttonColourId, Colours::lightgrey);  // Reset color
    }
}
