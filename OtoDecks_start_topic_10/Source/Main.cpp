/*
==============================================================================
    This file was auto-generated!
    It contains the basic startup code for a JUCE application.
==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

//==============================================================================
// OtoDecksApplication: This is the main JUCE application class responsible for
// starting and managing the application's lifecycle.
class OtoDecksApplication : public JUCEApplication
{
public:
    //==============================================================================
    OtoDecksApplication() {}
    
    // Returns the application name (used in window titles and other places)
    const String getApplicationName() override       
    { 
        return ProjectInfo::projectName; 
    }

    // Returns the version of the application
    const String getApplicationVersion() override    
    { 
        return ProjectInfo::versionString; 
    }

    // Allows multiple instances of the application to run at the same time
    bool moreThanOneInstanceAllowed() override       
    { 
        return true; 
    }

    //==============================================================================
    // Called when the application is launched. This is where you initialize your
    // application and create the main window.
    void initialise(const String& commandLine) override
    {
        // Create the main window, setting the application name as the window title
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    // Called when the application is shutting down. Clean up any resources here.
    void shutdown() override
    {
        // Destroy the main window
        mainWindow = nullptr;
    }

    //==============================================================================
    // Called when the system requests the application to quit.
    void systemRequestedQuit() override
    {
        // Allow the application to quit by calling quit()
        quit();
    }

    // Called when another instance of the application is started (if allowed)
    void anotherInstanceStarted(const String& commandLine) override
    {
        // Handle new instances of the application (if necessary)
    }

    //==============================================================================
    // MainWindow class: This class creates the main window and contains the
    // content component (MainComponent) that holds the application's user interface.
    class MainWindow : public DocumentWindow
    {
    public:
        // Constructor: Initializes the window with the given name (title).
        MainWindow(String name)  
            : DocumentWindow(name,
                             Desktop::getInstance().getDefaultLookAndFeel()
                                 .findColour(ResizableWindow::backgroundColourId),
                             DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);  // Set the main content component

            #if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);  // Fullscreen mode for mobile devices
            #else
            setResizable(true, true);  // Enable window resizing for desktop
            centreWithSize(getWidth(), getHeight());  // Center the window on the screen
            #endif

            setVisible(true);  // Make the window visible
        }

        // Called when the close button is pressed (e.g., the "X" in the window title bar)
        void closeButtonPressed() override
        {
            // Request the application to quit when the window is closed
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

private:
    // A unique pointer to manage the main window
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() function that launches the application.
START_JUCE_APPLICATION(OtoDecksApplication)
