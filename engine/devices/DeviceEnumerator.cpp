#include "DeviceEnumerator.h"

#include <juce_audio_devices/juce_audio_devices.h>

namespace audio_blade::devices
{
namespace
{

AudioDeviceDirection inferDirection(const juce::AudioIODeviceType& type, const juce::String& deviceName)
{
    const auto hasInputs = type.hasSeparateInputsAndOutputs()
                               ? type.getDeviceNames(true).contains(deviceName)
                               : type.getDeviceNames().contains(deviceName);
    const auto hasOutputs = type.hasSeparateInputsAndOutputs()
                                ? type.getDeviceNames(false).contains(deviceName)
                                : type.getDeviceNames().contains(deviceName);

    if (hasInputs && hasOutputs)
        return AudioDeviceDirection::duplex;
    if (hasInputs)
        return AudioDeviceDirection::input;
    if (hasOutputs)
        return AudioDeviceDirection::output;
    return AudioDeviceDirection::unknown;
}

AudioDeviceInfo makeInfo(
    const juce::AudioIODeviceType& type,
    const juce::String& deviceName,
    const juce::String& defaultInputName,
    const juce::String& defaultOutputName)
{
    AudioDeviceInfo info;
    info.name = deviceName.toStdString();
    info.identifier = (type.getTypeName() + "::" + deviceName).toStdString();
    info.direction = inferDirection(type, deviceName);
    info.isDefaultInput = defaultInputName == deviceName;
    info.isDefaultOutput = defaultOutputName == deviceName;

    std::unique_ptr<juce::AudioIODevice> probeDevice;
    if (type.hasSeparateInputsAndOutputs())
        probeDevice.reset(type.createDevice(deviceName, deviceName));
    else
        probeDevice.reset(type.createDevice(deviceName, {}));

    if (probeDevice != nullptr)
    {
        info.inputChannelCount = probeDevice->getInputChannelNames().size();
        info.outputChannelCount = probeDevice->getOutputChannelNames().size();

        const auto currentRate = probeDevice->getCurrentSampleRate();
        if (currentRate > 0.0)
            info.currentSampleRate = currentRate;

        for (const auto rate : probeDevice->getAvailableSampleRates())
            info.availableSampleRates.push_back(rate);

        for (const auto bufferSize : probeDevice->getAvailableBufferSizes())
            info.availableBufferSizes.push_back(bufferSize);
    }

    return info;
}

} // namespace

std::vector<AudioDeviceInfo> DeviceEnumerator::enumerateDevices() const
{
    std::vector<AudioDeviceInfo> devices;

    juce::AudioDeviceManager manager;
    manager.createAudioDeviceTypes(manager.getAvailableDeviceTypes());

    const auto currentSetup = manager.getAudioDeviceSetup();
    const auto defaultInputName = currentSetup.inputDeviceName;
    const auto defaultOutputName = currentSetup.outputDeviceName;

    for (auto* type : manager.getAvailableDeviceTypes())
    {
        if (type == nullptr)
            continue;

        type->scanForDevices();

        if (type->hasSeparateInputsAndOutputs())
        {
            for (const auto& inputName : type->getDeviceNames(true))
                devices.push_back(makeInfo(*type, inputName, defaultInputName, defaultOutputName));

            for (const auto& outputName : type->getDeviceNames(false))
            {
                const auto alreadyIncluded = std::any_of(
                    devices.begin(),
                    devices.end(),
                    [&](const auto& existing) { return existing.name == outputName.toStdString(); });
                if (!alreadyIncluded)
                    devices.push_back(makeInfo(*type, outputName, defaultInputName, defaultOutputName));
            }
        }
        else
        {
            for (const auto& deviceName : type->getDeviceNames())
                devices.push_back(makeInfo(*type, deviceName, defaultInputName, defaultOutputName));
        }
    }

    return devices;
}

} // namespace audio_blade::devices
