#pragma once

#include "AudioDeviceInfo.h"

#include <vector>

namespace audio_blade::devices
{

class DeviceEnumerator
{
public:
    [[nodiscard]] std::vector<AudioDeviceInfo> enumerateDevices() const;
};

} // namespace audio_blade::devices
