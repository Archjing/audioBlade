#include "engine/devices/DeviceEnumerator.h"

#include <juce_gui_extra/juce_gui_extra.h>

#if JUCE_WINDOWS
#include <io.h>
#include <windows.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>

namespace
{

void prepareConsoleForUtf8()
{
#if JUCE_WINDOWS
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

void emitSummaryToStdout(const juce::String& summary)
{
#if JUCE_WINDOWS
    const auto stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    const auto isConsole = stdoutHandle != nullptr
                           && stdoutHandle != INVALID_HANDLE_VALUE
                           && _isatty(_fileno(stdout)) != 0;

    if (isConsole)
    {
        DWORD written = 0;
        WriteConsoleW(stdoutHandle, summary.toWideCharPointer(), static_cast<DWORD>(summary.length()), &written, nullptr);
        return;
    }
#endif

    const auto* utf8 = summary.toRawUTF8();
    const auto utf8Bytes = static_cast<size_t>(summary.getNumBytesAsUTF8() - 1);
    std::fwrite(utf8, 1, utf8Bytes, stdout);
}

juce::String boolToString(const bool value)
{
    return value ? "true" : "false";
}

juce::String buildDeviceSummary()
{
    audio_blade::devices::DeviceEnumerator enumerator;
    const auto devices = enumerator.enumerateDevices();

    juce::String output;
    output << "audioBlade Phase 0 device summary\n";
    output << "date=" << juce::Time::getCurrentTime().toISO8601(true) << "\n";
    output << "os=" << juce::SystemStats::getOperatingSystemName() << "\n";
    output << "device_count=" << juce::String(static_cast<int>(devices.size())) << "\n";

    std::map<juce::String, std::vector<audio_blade::devices::AudioDeviceInfo>> devicesByBackend;
    for (const auto& device : devices)
        devicesByBackend[device.backendType].push_back(device);

    for (const auto& [backendType, backendDevices] : devicesByBackend)
    {
        output << "backend=" << backendType << "\n";

        for (const auto& device : backendDevices)
        {
            output << "- name=" << device.name
                   << " direction=" << audio_blade::devices::toString(device.direction)
                   << " input_channels=" << juce::String(device.inputChannelCount)
                   << " output_channels=" << juce::String(device.outputChannelCount)
                   << " default_input=" << boolToString(device.isDefaultInput)
                   << " default_output=" << boolToString(device.isDefaultOutput);

            if (device.currentSampleRate.has_value())
                output << " current_sample_rate=" << juce::String(*device.currentSampleRate);

            output << "\n";
        }
    }

    return output;
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
            prepareConsoleForUtf8();
            emitSummaryToStdout(buildDeviceSummary());
            std::cout.flush();
            std::fflush(stdout);
            std::exit(0);
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
