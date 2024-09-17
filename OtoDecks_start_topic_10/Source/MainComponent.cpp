/*
==============================================================================
    This file was auto-generated!
==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
// Constructor: Initializes the main component of the application, adds 3 decks,
// playlist, and sets up audio mixing. (PERSONAL CONTRIBUTION: Added third deck, 
// theme toggle button, and audio mixing setup.)
MainComponent::MainComponent()
    : AudioAppComponent(),
      player1{formatManager},
      deckGUI1{&player1, formatManager, thumbCache, &playlistComponent},
      player2{formatManager},
      deckGUI2{&player2, formatManager, thumbCache, &playlistComponent},
      player3{formatManager},  // Added third deck (PERSONAL CONTRIBUTION)
      deckGUI3{&player3, formatManager, thumbCache, &playlistComponent},  // Third deck GUI
      playlistComponent(&player1)  // Pass player1 to PlaylistComponent
{
    // Set the size of the main window
    setSize(800, 600);

    // Check for audio recording permissions
    if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio)
        && !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request(RuntimePermissions::recordAudio,
            [&](bool granted) { if (granted) setAudioChannels(2, 2); });
    }
    else
    {
        setAudioChannels(0, 2);  // Specify 0 input channels and 2 output channels
    }

    // Make the decks and playlist component visible
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(deckGUI3);  // Add third deck to the GUI (PERSONAL CONTRIBUTION)
    addAndMakeVisible(playlistComponent);

    // Make the theme toggle button visible and set its click behavior
    addAndMakeVisible(themeToggleButton);  // (PERSONAL CONTRIBUTION)
    themeToggleButton.onClick = [this]() { toggleTheme(); };  // Toggle theme on button click

    // Register audio formats and start mixing audio from the three players
    formatManager.registerBasicFormats();
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    mixerSource.addInputSource(&player3, false);  // Add third player to the mixer (PERSONAL CONTRIBUTION)

    applyTheme();  // Apply the initial theme (default is Light)
}

MainComponent::~MainComponent()
{
    shutdownAudio();  // Shut down the audio system when the component is destroyed
}

//==============================================================================
// Toggle between Light and Dark themes (PERSONAL CONTRIBUTION)
void MainComponent::toggleTheme()
{
    currentTheme = (currentTheme == Theme::Light) ? Theme::Dark : Theme::Light;
    applyTheme();  // Apply the newly selected theme
}

// Apply the current theme to the component and child components (PERSONAL CONTRIBUTION)
void MainComponent::applyTheme()
{
    if (currentTheme == Theme::Light)
    {
        getLookAndFeel().setColour(ResizableWindow::backgroundColourId, juce::Colours::white);
        getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::grey);
        getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::lightgrey);
        getLookAndFeel().setColour(juce::Label::textColourId, juce::Colours::black);
    }
    else
    {
        getLookAndFeel().setColour(ResizableWindow::backgroundColourId, juce::Colours::black);
        getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::white);
        getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
        getLookAndFeel().setColour(juce::Label::textColourId, juce::Colours::white);
    }
    repaint();  // Repaint the component to reflect the new theme
}

//==============================================================================
// Prepare the audio players and mixer for playback (called before playback starts)
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player3.prepareToPlay(samplesPerBlockExpected, sampleRate);  // Prepare third deck for playback (PERSONAL CONTRIBUTION)

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.removeAllInputs();  // Ensure no duplicate inputs
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    mixerSource.addInputSource(&player3, false);  // Add third player to the mixer (PERSONAL CONTRIBUTION)
}

//==============================================================================
// Retrieve the next block of audio data and pass it to the mixer source
void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

//==============================================================================
// Release the audio resources when they are no longer needed
void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    player3.releaseResources();  // Release resources for the third player (PERSONAL CONTRIBUTION)
    mixerSource.releaseResources();
}

//==============================================================================
// Paint method to fill the background with the current theme's color
void MainComponent::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

//==============================================================================
// Resize and position the components (3 decks, playlist, and theme toggle button)
void MainComponent::resized()
{
    int deckWidth = getWidth() / 3;  // Divide the window width into thirds for the decks
    int deckHeight = getHeight() / 2;  // Use the top half for the decks
    int playlistHeight = 150;  // Adjust as needed

    // Set bounds for each deck
    deckGUI1.setBounds(0, 0, deckWidth, deckHeight);
    deckGUI2.setBounds(deckWidth, 0, deckWidth, deckHeight);
    deckGUI3.setBounds(2 * deckWidth, 0, deckWidth, deckHeight);  // Third deck (PERSONAL CONTRIBUTION)

    // Set bounds for the playlist component at the bottom of the window
    playlistComponent.setBounds(0, getHeight() - playlistHeight, getWidth(), playlistHeight);

    // Set bounds for the theme toggle button (centered horizontally at the bottom)
    themeToggleButton.setBounds((getWidth() / 2) - 50, getHeight() - 40, 100, 30);  // Theme toggle button (PERSONAL CONTRIBUTION)
}
