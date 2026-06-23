# JUCE Official Documentation Reference

Purpose: keep a local reference index to the official JUCE documentation that is most relevant to audioBlade development.

Last refreshed: 2026-06-23

## Official Entry Points

- JUCE documentation landing page:
  - https://juce.com/learn/documentation/
- Latest stable API docs:
  - https://docs.juce.com/master/
- Latest development API docs:
  - https://docs.juce.com/develop/

Use `master` as the default reference unless audioBlade intentionally tracks JUCE `develop`.

## Local Source Snapshot

The repository currently vendors JUCE at:

- `extern/JUCE`

Current pinned checkout:

- `extern/JUCE` git commit: `2cdfca8feb300fb424002ba2c2751569e5bacb64`

Useful local JUCE docs shipped with the source tree:

- `extern/JUCE/README.md`
- `extern/JUCE/docs/README.md`
- `extern/JUCE/docs/CMake API.md`
- `extern/JUCE/docs/Linux Dependencies.md`

These local docs are useful for source-aligned reading. The online docs remain the primary API reference.

## Priority References For audioBlade

These are the first pages to consult when writing or reviewing Phase 0 and Phase 1 code.

### Application bootstrap

- `juce::JUCEApplication`
  - https://docs.juce.com/master/classjuce_1_1JUCEApplication.html

Why it matters:

- app startup and shutdown
- command-line handling
- event-loop ownership
- the `START_JUCE_APPLICATION` entry path used in `apps/desktop/main.cpp`

### Audio device orchestration

- `juce::AudioDeviceManager`
  - https://docs.juce.com/master/classjuce_1_1AudioDeviceManager.html

Why it matters:

- current-device setup
- input/output selection
- creating and tracking available device types
- the main JUCE surface for device enumeration before real routing exists

### Audio backend and device-type discovery

- `juce::AudioIODeviceType`
  - https://docs.juce.com/master/classjuce_1_1AudioIODeviceType.html

Why it matters:

- device-type enumeration
- device-name listing
- default-device indices
- device creation rules
- platform backend mapping such as WASAPI, ASIO, CoreAudio, ALSA

Important note for current code:

- `AudioDeviceManager::createAudioDeviceTypes()` populates device-type containers
- `AudioIODeviceType::createDevice(...)` is non-const

### Audio device inspection

- `juce::AudioIODevice`
  - https://docs.juce.com/master/classjuce_1_1AudioIODevice.html

Why it matters:

- input/output channel names
- sample rates
- buffer sizes
- current device state
- the low-level data audioBlade needs for diagnostics output

### JUCE string usage

- `juce::String`
  - https://docs.juce.com/master/classjuce_1_1String.html

Why it matters:

- UTF handling
- conversion to and from `std::string`
- string matching and logging in cross-platform code

## CMake And Module References

### Documentation overview

- JUCE docs home:
  - https://docs.juce.com/

### CMake-facing module docs

- `juce_audio_devices.h` file reference:
  - https://docs.juce.com/master/juce__audio__devices_8h.html
- `juce_gui_extra.h` file reference:
  - https://docs.juce.com/master/juce__gui__extra_8h.html

Why these matter:

- module-level build notes
- platform-specific compile requirements
- CMake-specific flags that are easy to miss on Windows

Windows-specific note already visible in the official docs:

- some JUCE features require extra CMake target options or NuGet setup on Windows
- WebView2 and Windows MIDI Services are examples of features that need extra project-level configuration

audioBlade should keep its first milestone narrow and avoid enabling optional JUCE subsystems until they are actually needed.

## Phase 0 Reading Order

For current audioBlade work, read in this order:

1. `juce::JUCEApplication`
2. `juce::AudioDeviceManager`
3. `juce::AudioIODeviceType`
4. `juce::AudioIODevice`
5. `extern/JUCE/docs/CMake API.md`
6. `extern/JUCE/docs/Linux Dependencies.md`

## Current Repository Relevance

These official docs map directly to the current audioBlade files:

- `apps/desktop/main.cpp`
  - `juce::JUCEApplication`
- `engine/devices/DeviceEnumerator.cpp`
  - `juce::AudioDeviceManager`
  - `juce::AudioIODeviceType`
  - `juce::AudioIODevice`
- `apps/desktop/CMakeLists.txt`
  - JUCE CMake API docs
- future diagnostics and route schema work
  - audio device metadata docs

## Usage Policy

Use this file as a local index, not as a frozen mirror of the JUCE documentation site.

When implementing or reviewing JUCE code:

- prefer the official online API docs for exact signatures
- use the vendored `extern/JUCE` source to confirm behavior against the pinned checkout
- record any JUCE-specific build or platform caveats back into repo-local docs when they affect audioBlade
