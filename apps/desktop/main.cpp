#include <juce_gui_extra/juce_gui_extra.h>

class AudioBladeApplication final : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "audioBlade"; }
    const juce::String getApplicationVersion() override { return "0.1.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String&) override
    {
        mainWindow = std::make_unique<MainWindow>(getApplicationName());
    }

    void shutdown() override
    {
        mainWindow.reset();
    }

    void systemRequestedQuit() override
    {
        quit();
    }

private:
    class MainComponent final : public juce::Component
    {
    public:
        MainComponent()
        {
            status.setText("audioBlade Phase 0 foundation", juce::dontSendNotification);
            status.setJustificationType(juce::Justification::centred);
            addAndMakeVisible(status);
            setSize(640, 360);
        }

        void resized() override
        {
            status.setBounds(getLocalBounds().reduced(24));
        }

    private:
        juce::Label status;
    };

    class MainWindow final : public juce::DocumentWindow
    {
    public:
        explicit MainWindow(juce::String name)
            : DocumentWindow(
                  std::move(name),
                  juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(
                      juce::ResizableWindow::backgroundColourId),
                  DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }
    };

    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(AudioBladeApplication)
