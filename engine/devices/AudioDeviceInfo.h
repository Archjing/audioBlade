#pragma once

#include <optional>
#include <string>
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

struct AudioDeviceInfo
{
    std::string name;
    std::string identifier;
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
