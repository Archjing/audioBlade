# Devices

Phase 0 Week 2 starts device-enumeration work here.

The Week 2 execution focus is:

- define the minimal `AudioDeviceInfo` model
- define the initial `DeviceEnumerator` abstraction
- integrate JUCE `AudioDeviceManager` for enumeration
- emit diagnostics outside the real-time audio path

The minimal cross-platform field set is:

- device name
- direction
- channel count
- default status
- sample rate or format capability when available

Week 2 non-goals:

- real audio forwarding
- virtual driver integration
- ASIO support
- AirPlay or network output
- real-time callback processing

Platform-specific fields may be added later in a diagnostics extension area, but the minimal field set above is the Week 2 baseline.
