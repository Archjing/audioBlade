# Profiles

This directory contains the Phase 0 profile model for future persistent routing.

The v0 schema is a serializable configuration contract only. It does not execute routes, install virtual devices, restore profiles, or claim real audio forwarding.

## Files

- `profile.schema.json`: JSON Schema for Profile v0.
- `examples/default-monitor-route.json`: example profile for one future stereo monitor route.

## V0 Scope

Profile v0 can describe:

- audio nodes with stable identifiers when available
- human-readable fallback matching
- stereo source and destination ports
- one or more serializable routes
- gain, mute, and sample-rate policy fields

Profile v0 does not include:

- real-time callback behavior
- virtual driver setup
- multichannel routing
- effects processing
- AirPlay, network sinks, or per-application routing
