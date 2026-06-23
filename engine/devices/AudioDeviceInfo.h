#pragma once

#include <juce_core/juce_core.h>

#include <optional>
#include <vector>

namespace audio_blade::devices
{

enum class AudioDeviceDirection
{
    input,
    output,
    duplex,
    unknown
};

[[nodiscard]] const char* toString(AudioDeviceDirection direction);

struct AudioDeviceInfo
{
    juce::String backendType;
    juce::String name;
    juce::String identifier;
    AudioDeviceDirection direction = AudioDeviceDirection::unknown;
    int inputChannelCount = 0;
    int outputChannelCount = 0;
    bool isDefaultInput = false;
    bool isDefaultOutput = false;
    std::optional<double> currentSampleRate;
    std::vector<double> availableSampleRates;
    std::vector<int> availableBufferSizes;
};

} // namespace audio_blade::devices
