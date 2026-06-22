# audioBlade Development Guidelines

## Purpose

This document defines project-level engineering rules for audioBlade. It turns the current technical reasoning into practical development constraints.

The detailed reasoning and references live in `docs/TECHNICAL_PLAN.md`.

## Product Boundary

audioBlade is a persistent desktop audio router for Windows and macOS.

The product must provide:

- Persistent system-level routing without requiring a DAW
- Routing between virtual audio endpoints and physical audio devices
- Saved profiles that can restore routes after restart or device reconnect
- A background audio engine independent of the GUI
- A UI for editing routes, gains, mute state, and profiles

The product must not depend on any DAW, editor, or foreground-only process to keep routing alive.

## Architecture Rules

Use this architecture split:

- `engine`: real-time audio routing, graph state, device bridge, profile restore
- `control`: Rust control service, route graph ownership, profiles, device policy, diagnostics
- `app/gui`: route editor and status UI
- `platform/windows`: WASAPI, ASIO, driver integration, Windows installer details
- `platform/macos`: Core Audio, AudioDriverKit/HAL integration, macOS packaging details
- `drivers`: third-party driver integration notes or first-party driver experiments
- `docs`: technical decisions, references, and risk notes
- `tools`: diagnostics, scripts, and test utilities

Keep the audio engine separate from the GUI. The GUI may crash or quit without stopping existing routes if the background engine is running.

For the first MVP, the control service may be implemented inside the JUCE app to reduce moving parts. Code should still keep the control-plane and audio-plane responsibilities separate so the Rust service can be split out later.

## Language And Runtime Rules

Default long-term stack:

- Use C++ for real-time audio engine work.
- Use JUCE for cross-platform audio device management and the first MVP GUI.
- Use Rust for the control service after the initial audio path is proven.
- Use Tauri only if a richer web-style GUI becomes more valuable than the smallest native JUCE app path.
- Use CMake for build orchestration.
- Use Python only for tooling, diagnostics, scripts, and tests.

Do not put real-time audio callback code in Python.

Do not rely on Python to expose OS-level virtual audio devices.

Do not pass normal real-time audio buffers through Rust, IPC, or the GUI. Rust may control routes and state, but C++ owns the real-time audio path.

MVP stack rule:

- Use C++/JUCE single-process implementation first if it gets the audio path validated faster.
- Split Rust control service later when profile management, diagnostics, update handling, and device policy become substantial enough to justify the boundary.

## Driver Dependency Rules

All default virtual audio driver dependencies must be open source and compatible with future commercial use.

Preferred licenses:

- MIT
- BSD
- Apache-2.0
- MS-PL
- MPL-2.0

Allowed only after explicit review:

- LGPL
- File-level copyleft where obligations are isolated and manageable

Do not use as default dependencies for a proprietary commercial build:

- GPL-2.0 drivers
- GPL-3.0 drivers
- Closed-source freeware
- Donationware drivers
- Proprietary virtual cable products

Before bundling any driver in a commercial installer, run a formal license review. This document is an engineering policy, not legal advice.

## Starting Driver Decisions

### Windows

Start with `VirtualDrivers/Virtual-Audio-Driver`.

Reasons:

- It targets Windows 10/11.
- It exposes a virtual speaker and virtual microphone.
- Its license path is more compatible with future commercial use: original code is MIT and SYSVAD-derived parts are under MS-PL notices.
- It aligns with a future SYSVAD-derived first-party Windows driver.
- It is faster for MVP validation than starting from raw Microsoft SYSVAD.

Do not start from raw SYSVAD unless the task is specifically first-party Windows driver R&D.

Treat VoiceMeeter as a Windows feature benchmark, not as a dependency.

Do not use VB-CABLE or VoiceMeeter as default dependencies because they are not open-source commercial dependencies.

### macOS

Start with `roc-streaming/roc-vad` as the first macOS validation candidate.

In this project, a spike means a short technical validation experiment, not a final product commitment.

The Roc VAD validation must answer:

- Can it create virtual audio endpoints on the target macOS versions?
- Can normal apps and DAWs see those endpoints?
- Can audioBlade route audio between Roc VAD endpoints and a physical device?
- Is latency acceptable for the first MVP?
- Does the device survive restart, relaunch, and reconnect scenarios?
- Is installation acceptable for users?
- Are MPL-2.0 obligations manageable for the intended commercial model?

Treat Soundflower only as a compatibility reference because it is older, despite its MIT license.

Treat BlackHole as a macOS feature benchmark, not as a default dependency. Do not bundle BlackHole into a proprietary commercial build unless audioBlade becomes GPL-3.0-compatible or a separate commercial license is obtained.

## MVP Order

Build in this order:

1. Windows device enumeration.
2. Windows route engine using `VirtualDrivers/Virtual-Audio-Driver`.
3. One stereo route from virtual input to physical output.
4. One stereo route from physical input to virtual output.
5. Profile save and restore.
6. Background engine lifecycle.
7. Device disconnect and reconnect recovery.
8. Basic route matrix UI.
9. macOS Roc VAD validation.
10. macOS route engine integration.

Do not start with AirPlay, network streaming, multi-receiver sync, or first-party drivers.

## First-Party Driver Policy

First-party virtual audio drivers are feasible but must not be the first milestone.

Start first-party driver work only when at least one of these is true:

- Third-party open-source drivers block commercial packaging.
- Endpoint reliability is not acceptable.
- Branding or installer requirements require owned drivers.
- Channel count, latency, or routing behavior cannot meet product requirements.
- The app has proven enough demand to justify several months of driver work.

Expected effort:

- Windows usable first-party driver: additional 3-6 months.
- macOS usable first-party driver: additional 4-8 months.
- Both platforms commercial-ready: realistically 9-18 months total depending on team size and testing depth.

Treat Windows and macOS drivers as separate projects. Do not assume one cross-platform driver layer exists.

## Real-Time Audio Rules

Real-time audio code must:

- Avoid memory allocation in audio callbacks.
- Avoid locks in audio callbacks unless proven bounded and safe.
- Avoid blocking I/O in audio callbacks.
- Keep callback work predictable and small.
- Separate control-plane changes from audio-thread processing.
- Handle sample-rate and buffer-size changes explicitly.
- Log diagnostics outside the real-time path.

Routing must account for:

- Sample-rate mismatch
- Buffer-size mismatch
- Multi-device clock drift
- Device disconnects
- Device ID changes across reboot or reconnect
- Exclusive/shared mode differences on Windows

## Routing Model Rules

Use a graph model, not hard-coded device pairs.

Required concepts:

- `AudioNode`: physical device, virtual device, app capture stream, network sink
- `Port`: input or output channel on a node
- `Route`: connection from source ports to destination ports
- `Profile`: named route set with gains, mutes, channel maps, and sample-rate policy

All routes must be serializable.

Profiles must use stable device identifiers where available, plus human-readable fallback matching when IDs change.

## Scope Control

MVP includes:

- Device listing
- Simple stereo routes
- Route persistence
- Background engine
- Basic GUI
- Device reconnect handling
- License-compatible virtual driver dependencies

MVP excludes:

- AirPlay streaming
- Per-application routing
- First-party virtual drivers
- Surround routing
- Effects processing
- Full VoiceMeeter replacement features
- Multi-receiver network sync

## Benchmark Products

Use these products only as behavior references:

- VoiceMeeter on Windows
- VB-CABLE on Windows
- BlackHole on macOS
- Soundflower on macOS historical compatibility
- TuneBlade for network/AirPlay-oriented behavior

Do not copy proprietary implementation details.

Do not make non-open-source products project dependencies.

## Documentation Rules

Every major technical decision must be recorded in `docs/TECHNICAL_PLAN.md` or a dedicated ADR-style document.

Driver-related decisions must include:

- Repository URL
- License URL
- Commercial-use assessment
- Functional gap assessment
- Packaging risks
- Decision status: candidate, accepted, rejected, or benchmark only

Use plain language for project decisions. Avoid ambiguous terms such as "spike" unless the document defines them.
