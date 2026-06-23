#include "engine/devices/DeviceEnumerator.h"

#include <juce_gui_extra/juce_gui_extra.h>

#include <iostream>
#include <sstream>

namespace
{

juce::String buildDeviceSummary()
{
    audio_blade::devices::DeviceEnumerator enumerator;
    const auto devices = enumerator.enumerateDevices();

    std::ostringstream output;
    output << "audioBlade Phase 0 device summary\n";
    output << "device_count=" << devices.size() << "\n";

    for (const auto& device : devices)
    {
        output << "- name=" << device.name
               << " direction=" << audio_blade::devices::toString(device.direction)
               << " input_channels=" << device.inputChannelCount
               << " output_channels=" << device.outputChannelCount
               << " default_input=" << (device.isDefaultInput ? "true" : "false")
               << " default_output=" << (device.isDefaultOutput ? "true" : "false");

        if (device.currentSampleRate.has_value())
            output << " current_sample_rate=" << *device.currentSampleRate;

        output << "\n";
    }

    return output.str();
}

bool hasDiagnoseDevicesArgument()
{
    const auto commandLine = juce::JUCEApplication::getCommandLineParameters();
    return commandLine.contains("--diagnose-devices");
}

} // namespace

class AudioBladeApplication final : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "audioBlade"; }
    const juce::String getApplicationVersion() override { return "0.1.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String&) override
    {
        if (hasDiagnoseDevicesArgument())
        {
            std::cout << buildDeviceSummary();
            quit();
            return;
        }

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
            status.setText(buildDeviceSummary(), juce::dontSendNotification);
            status.setJustificationType(juce::Justification::centred);
            status.setMinimumHorizontalScale(0.75f);
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
