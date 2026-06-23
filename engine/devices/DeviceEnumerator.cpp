#include "DeviceEnumerator.h"

#include <juce_audio_devices/juce_audio_devices.h>

#include <set>

namespace audio_blade::devices
{
const char* toString(const AudioDeviceDirection direction)
{
    switch (direction)
    {
        case AudioDeviceDirection::input:
            return "input";
        case AudioDeviceDirection::output:
            return "output";
        case AudioDeviceDirection::duplex:
            return "duplex";
        case AudioDeviceDirection::unknown:
            return "unknown";
    }

    return "unknown";
}

namespace
{

AudioDeviceDirection inferDirection(juce::AudioIODeviceType& type, const juce::String& deviceName)
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
    juce::AudioIODeviceType& type,
    const juce::String& deviceName,
    const juce::String& defaultInputName,
    const juce::String& defaultOutputName)
{
    AudioDeviceInfo info;
    const auto safeName = deviceName.isNotEmpty() ? deviceName : juce::String("[unnamed-device]");
    info.backendType = type.getTypeName();
    info.name = safeName;
    info.identifier = type.getTypeName() + "::" + deviceName;
    info.direction = inferDirection(type, safeName);
    info.isDefaultInput = defaultInputName == safeName;
    info.isDefaultOutput = defaultOutputName == safeName;

    std::unique_ptr<juce::AudioIODevice> probeDevice;
    if (type.hasSeparateInputsAndOutputs())
        probeDevice.reset(type.createDevice(safeName, safeName));
    else
        probeDevice.reset(type.createDevice(safeName, {}));

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
    juce::OwnedArray<juce::AudioIODeviceType> deviceTypes;
    manager.createAudioDeviceTypes(deviceTypes);

    const auto currentSetup = manager.getAudioDeviceSetup();
    const auto defaultInputName = currentSetup.inputDeviceName;
    const auto defaultOutputName = currentSetup.outputDeviceName;

    for (auto* type : deviceTypes)
    {
        if (type == nullptr)
            continue;

        type->scanForDevices();
        std::set<juce::String> seenIdentifiers;

        if (type->hasSeparateInputsAndOutputs())
        {
            for (const auto& inputName : type->getDeviceNames(true))
            {
                const auto key = type->getTypeName() + "::" + inputName;
                if (seenIdentifiers.insert(key).second)
                    devices.push_back(makeInfo(*type, inputName, defaultInputName, defaultOutputName));
            }

            for (const auto& outputName : type->getDeviceNames(false))
            {
                const auto key = type->getTypeName() + "::" + outputName;
                if (seenIdentifiers.insert(key).second)
                    devices.push_back(makeInfo(*type, outputName, defaultInputName, defaultOutputName));
            }
        }
        else
        {
            for (const auto& deviceName : type->getDeviceNames())
            {
                const auto key = type->getTypeName() + "::" + deviceName;
                if (seenIdentifiers.insert(key).second)
                    devices.push_back(makeInfo(*type, deviceName, defaultInputName, defaultOutputName));
            }
        }
    }

    return devices;
}

} // namespace audio_blade::devices
