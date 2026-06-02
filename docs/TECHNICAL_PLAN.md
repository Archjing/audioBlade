# audioBlade Technical Plan

## Goal

Build a Windows + macOS desktop audio routing app with TuneBlade-like persistence: audio routing keeps working after a DAW such as Studio One is closed.

The product should let users route audio between:

- Physical audio interfaces, including external USB/Thunderbolt sound cards
- System output and app playback
- App or device inputs
- Virtual inputs and virtual outputs
- Optional network outputs later, such as AirPlay-like streaming

## Core Product Requirement

Studio One can act as an audio patch bay only while it is running. To keep routing alive after it exits, audioBlade needs its own persistent audio graph.

That means the minimum architecture is:

- A background audio engine process or service
- One or more virtual audio devices exposed to the OS
- A GUI that edits routing state
- Saved routing profiles restored on login/device reconnect

A normal Python app cannot by itself expose a stable system audio device. Python can control UI, configuration, or orchestration, but the real-time audio path should be native code. Windows virtual audio devices are driver-level components, with Microsoft's SYSVAD sample being the relevant reference path for a first-party virtual audio driver.[2] macOS virtual audio devices similarly require Core Audio or AudioDriverKit-level components rather than ordinary app-only code.[3][4]

## Open-Source Driver Dependency Policy

Virtual audio drivers used by the MVP must be open source and compatible with future commercial use. For a proprietary commercial audioBlade release, prefer permissive or weak-copyleft licenses.

Default policy:

- Preferred: MIT, BSD, Apache-2.0, MS-PL, MPL-2.0.
- Acceptable with review: LGPL, file-level copyleft where dynamic linking or file-level source obligations are manageable.
- Avoid as default dependency for proprietary builds: GPL-2.0/GPL-3.0 drivers unless audioBlade itself is GPL-compatible or the driver author grants a separate commercial license.
- Avoid as default dependency: closed-source freeware, donationware, and proprietary virtual cable drivers, even if they are convenient for personal use.

This is an engineering compatibility screen, not legal advice. Before bundling a driver in a commercial installer, run a formal license review.

## Driver Capability Comparison

The license-compatible open-source candidates are good enough for an MVP only if audioBlade itself provides the persistent routing engine, mixer state, profile management, device reconnection logic, and UI. They are not equivalent to mature end-user products such as VoiceMeeter or BlackHole out of the box.

## Recommended Starting Drivers

Start with one driver per platform. Do not begin by supporting every candidate, because early work should validate audioBlade's routing graph, profile persistence, and device-reconnect behavior.

### Windows Starting Driver

Use `VirtualDrivers/Virtual-Audio-Driver` first.[9][10]

Reasons:

- It already targets Windows 10/11.
- It exposes both a virtual speaker and virtual microphone, which matches the initial audioBlade routing use case.
- It is based on Microsoft's SYSVAD direction, so future migration toward a first-party SYSVAD-derived driver is conceptually aligned.
- The repository states original code is MIT and Microsoft sample-derived parts are MS-PL, which is compatible with the commercial-use screening policy in this plan.
- It is closer to a working end-user virtual endpoint than starting from raw SYSVAD.

Do not start from raw SYSVAD unless the first milestone is specifically driver R&D. SYSVAD is the right reference for a future first-party driver, but it will slow down validation of the product experience.[2]

### macOS Starting Driver

Use `roc-streaming/roc-vad` first as a research/MVP candidate.[13]

Reasons:

- It is MPL-2.0, which is more manageable for a proprietary commercial app than GPL-licensed drivers, subject to formal legal review.
- It is a modern macOS virtual audio device project rather than a legacy-only compatibility path.
- It supports creating and controlling virtual devices and exposes programmatic control, which is useful for an audioBlade-managed routing layer.
- It gives a realistic path to validate installation, device persistence, and engine integration without depending on BlackHole's GPL-3.0 license.

Treat `Soundflower` only as a fallback compatibility spike.[14] Its MIT license is attractive, but it is older and should not define the main macOS route.

### First MVP Order

Build and test in this order:

1. Windows + VirtualDrivers/Virtual-Audio-Driver.
2. audioBlade route engine: one virtual input to one physical output.
3. Profile persistence and auto-restore.
4. Device reconnect handling.
5. macOS + Roc VAD spike.
6. Compare both platforms against VoiceMeeter/BlackHole workflows for functional gaps.

This order prioritizes the most direct Windows path first. It also avoids sinking early time into macOS driver packaging before the routing product model is proven.

### Windows

Compared with VB-CABLE:

- Gap: moderate.
- VB-CABLE is mainly a simple virtual cable where audio sent to the cable input is forwarded to the cable output.[12]
- VirtualDrivers/Virtual-Audio-Driver provides the same broad category of capability: a Windows virtual speaker and virtual microphone visible to applications.[9]
- The practical gap is likely installer polish, maturity, support history, and edge-case stability rather than the basic concept.

Compared with VoiceMeeter:

- Gap: large.
- VoiceMeeter is not just a virtual driver. It is a full virtual mixer with hardware inputs, software inputs, multi-bus routing, effects, macro control, virtual ASIO, multichannel support, and network streaming features.[17][18]
- Open-source driver candidates do not replace this product-level mixer. audioBlade would need to implement routing, mixing, gain, mute, profile persistence, and diagnostics itself.
- For the first milestone, that gap is acceptable because the MVP needs virtual endpoints plus a basic stereo router. For a production replacement of VoiceMeeter Banana/Potato, the gap is substantial.

Windows recommendation:

- Use VirtualDrivers/Virtual-Audio-Driver or a SYSVAD-derived first-party driver only as the virtual endpoint layer.
- Do not expect the driver dependency to provide the full product experience.
- Treat VoiceMeeter as a feature benchmark, not as an architectural dependency.

### macOS

Compared with BlackHole:

- Gap: moderate to large.
- BlackHole is a modern macOS virtual loopback driver that explicitly targets app-to-app audio pass-through with no additional latency and supports multiple channel configurations.[15]
- Roc VAD is modern and MPL-2.0, but its primary focus is virtual-device streaming to and from Roc Toolkit-compatible remote peers rather than being a general-purpose local loopback cable.[13]
- Soundflower is MIT and historically useful, but it is older and should not be treated as the modern default for current macOS releases.[14]

macOS recommendation:

- Use Roc VAD as the first modern license-compatible research candidate.
- Use Soundflower only for historical comparison or compatibility testing.
- Treat BlackHole as the functional benchmark. Do not bundle it in a proprietary commercial build unless audioBlade is GPL-3.0-compatible or a separate commercial license is obtained from the author.[15]

### Product Impact

The open-source candidates can cover the lowest layer: exposing virtual audio endpoints. They do not fully cover:

- Polished routing matrix
- Per-route gain/mute
- Device reconnect recovery
- Profile management
- ASIO-grade Windows workflows
- Multi-client DAW behavior
- End-user installer quality
- Diagnostics for sample-rate mismatch and clock drift

Therefore, the product strategy should be:

1. Use license-compatible open-source drivers only as MVP virtual endpoint dependencies.
2. Build audioBlade's own routing engine and UI above them.
3. Plan a first-party virtual driver if endpoint reliability or commercial packaging becomes a blocker.
4. Benchmark against VoiceMeeter on Windows and BlackHole/Loopback-style workflows on macOS, while avoiding incompatible dependencies.

## First-Party Virtual Driver Feasibility

Self-developed virtual audio drivers are feasible, but they should not be the first milestone. The engineering difficulty is not just audio DSP. The main cost is OS integration, driver lifecycle, signing/notarization, installers, compatibility testing, and support across OS updates.

### Windows Feasibility

Technical path:

- Use Microsoft's SYSVAD sample as the starting point for a virtual audio endpoint driver.[2]
- Customize endpoint topology, channel layouts, names, formats, and routing bridge to the audioBlade engine.
- Build a signed driver package and installer.[6][11]
- Test on Windows 10 and Windows 11 across common audio APIs: WASAPI shared, WASAPI exclusive, DirectSound compatibility, and DAW behavior.

Estimated effort:

- Research spike: 2-4 weeks.
- Internal prototype driver: 1-2 months.
- Usable MVP driver with installer: 2-4 months.
- Commercial-grade driver with signing, update handling, crash diagnostics, and broad hardware/app compatibility: 4-9+ months.

Primary risks:

- Driver signing and distribution pipeline.
- BSOD/kernel stability risk if kernel-mode code is touched incorrectly.
- App compatibility edge cases across DAWs, conferencing apps, browsers, and games.
- Multi-client behavior and format negotiation.
- Debugging cost is much higher than normal app debugging.

Windows conclusion:

- Feasible, but expensive.
- Starting from SYSVAD is realistic.
- For a solo developer or very small team, expect several months before it is stable enough for paid users.

### macOS Feasibility

Technical path:

- Build a virtual audio device using the current Apple-supported audio-driver path for target macOS versions: AudioDriverKit where applicable, or Core Audio HAL/AudioServerPlugIn-style implementation where still suitable.[3][4]
- Implement virtual input/output streams and bridge them to audioBlade's engine.
- Handle bundle identifiers, entitlements, user approval, notarization, and installer flow.[7]
- Test across current macOS versions and Apple Silicon/Intel if both are supported.

Estimated effort:

- Research spike: 3-6 weeks.
- Internal prototype virtual device: 1-3 months.
- Usable MVP driver/plugin with installer: 3-5 months.
- Commercial-grade implementation with notarization, user approval flow, device recovery, and OS-update resilience: 6-12+ months.

Primary risks:

- Apple platform rules and driver model changes.
- Entitlements and installer approval friction.
- Device disappearance or permission issues after OS updates.
- Clocking, sample-rate negotiation, and aggregate/multi-output behavior.
- Fewer modern permissive-license examples than desired.

macOS conclusion:

- Feasible, but usually harder to productize than the basic prototype suggests.
- The license-compatible dependency path is useful to validate product behavior before committing to this cost.

### Practical Recommendation

Do not start with first-party drivers unless the product requirement is explicitly "ship without third-party virtual-device dependencies." The better sequence is:

1. Build the routing engine and UI using license-compatible existing drivers.
2. Measure where the dependency blocks commercial quality: latency, channel count, installer, stability, branding, or support.
3. Start first-party driver spikes in parallel only after the routing product has proven demand.
4. Treat Windows and macOS drivers as separate projects, not a shared cross-platform module.

Expected schedule if audioBlade includes first-party drivers on both platforms:

- MVP app without first-party drivers: 1-3 months.
- Windows first-party driver added: additional 3-6 months for usable quality.
- macOS first-party driver added: additional 4-8 months for usable quality.
- Both platforms commercial-ready: realistically 9-18 months depending on team size, testing depth, and signing/notarization friction.

## Recommended Language And Stack

### Current stack decision

The recommended long-term stack is a hybrid architecture:

- Real-time audio engine: C++ with JUCE.
- Control service: Rust.
- GUI: JUCE for fastest MVP, or Tauri for a more web-style product UI after the engine is stable.
- Windows backend: WASAPI first, ASIO optional, custom driver later.
- macOS backend: Core Audio HAL / AudioServerPlugIn, DriverKit later if required.
- Config/state: JSON or SQLite.
- Scripting/prototyping: Python only for tooling and test utilities.

### Why hybrid C++ + Rust

C++ remains the safest default for the real-time audio path because JUCE provides mature cross-platform audio device management, callback handling, buffer processing, and desktop audio-app infrastructure.[5]

Rust is valuable in the non-real-time control layer:

- Route graph state.
- Profile persistence.
- Device reconnect policy.
- IPC.
- Logging and diagnostics.
- Update orchestration.
- Configuration validation.
- Background service lifecycle.

The key boundary is that audio buffers must not cross the Rust/C++ or process boundary in normal operation. Rust should send control messages, not process real-time audio buffers.

### MVP stack

Use a C++/JUCE single-process prototype first if speed is the priority. This keeps the initial build small, avoids cross-language complexity, and validates the audio path quickly.

After the audio path works, split out the Rust control service if profile management, device state, diagnostics, and update behavior become complex enough to justify the boundary.

### Final product stack

For the commercial product, prefer:

```text
C++/JUCE audio engine
  - real-time audio callbacks
  - device I/O
  - low-latency routing
  - platform audio API integration

Rust control service
  - route graph ownership
  - profiles
  - device reconnect state machine
  - diagnostics
  - update and lifecycle control

GUI
  - JUCE for smallest native app path
  - Tauri if richer UI iteration and web-style interface become more valuable
```

This split should not materially reduce runtime performance as long as the real-time audio path stays inside the C++ engine. The main installation/runtime overhead comes from virtual audio drivers and, if chosen, a Tauri WebView dependency, not from Rust itself.

### Why not Python for the engine

Python is acceptable for prototypes, but not ideal for this product's core audio path because:

- Low-latency callback audio needs predictable timing
- Exposing OS audio devices requires native driver/plugin components
- Packaging privileged audio components is platform-specific
- GIL and dependency packaging increase risk in real-time paths

Python can still be useful for CLI diagnostics, integration tests, and development tooling.

## Architecture

```text
+--------------------+        +---------------------------+
| Desktop GUI        | <----> | Rust Control Service      |
| route editor       | IPC    | profiles, state, policy   |
+--------------------+        +-------------+-------------+
                                           |
                                           | control messages only
                                           v
                              +------------+-------------+
                              | C++/JUCE Audio Engine    |
                              | real-time routing graph  |
                              +------------+-------------+
                                           |
                         +-----------------+-----------------+
                         |                                   |
                +--------v--------+                 +--------v--------+
                | Virtual Device  |                 | Physical Device |
                | input/output    |                 | external card   |
                +-----------------+                 +-----------------+
```

For the first MVP, the Rust control service can be collapsed into the JUCE app to reduce implementation overhead. The architectural boundary should still be kept in code so that the service can be split out later.

## Persistent Routing Model

Use a graph model instead of hard-coded pairs.

Entities:

- `AudioNode`: physical device, virtual device, app capture stream, network sink
- `Port`: input or output channel on a node
- `Route`: connection from source port(s) to destination port(s)
- `Profile`: named set of routes, channel maps, gains, and sample-rate policy

Example profile:

```json
{
  "name": "Studio Monitor",
  "routes": [
    {
      "source": "virtual:AudioBlade Input:1-2",
      "destination": "device:External Interface Out:1-2",
      "gainDb": 0.0
    },
    {
      "source": "device:External Interface In:1-2",
      "destination": "virtual:AudioBlade Mic:1-2",
      "gainDb": 0.0
    }
  ]
}
```

## Windows Implementation

### Phase 1: User-mode MVP

Use existing virtual audio cable software for initial validation:

- Windows default candidate: VirtualDrivers/Virtual-Audio-Driver. It exposes a virtual speaker and virtual microphone for Windows 10/11, is marked MIT for original code, and includes Microsoft SYSVAD-derived third-party code under MS-PL notices.[9][10]
- Windows fallback/reference candidate: Microsoft's SYSVAD sample directly. It is MS-PL and useful as the base for a first-party driver, but it is a sample rather than a product-ready virtual cable.[2][11]
- Not default: VB-CABLE and VoiceMeeter-style drivers, because they are not open-source dependencies suitable for this project's default commercial path.[12]
- audioBlade controls routing between those devices and physical interfaces.

Use WASAPI shared mode first. WASAPI supports device-level capture/render flows, and Microsoft documents loopback recording for capturing audio rendered by an endpoint.[1]

- Enumerate render/capture devices
- Capture from virtual input or physical input
- Render to physical output or virtual output
- Persist profiles and auto-start engine on login

This gets a usable route manager without writing a driver immediately.

### Phase 2: Better latency and pro audio support

Add:

- WASAPI exclusive mode where safe
- ASIO support for pro external interfaces
- Sample-rate conversion when source and destination differ
- Drift correction when routing between independent hardware clocks

### Phase 3: Own Windows virtual audio driver

Options:

- Build a custom virtual audio driver based on Microsoft's SYSVAD sample.[2]
- Driver signing and installer pipeline are required.[6]
- Expect this to be the highest-cost Windows task

This is necessary only if audioBlade must ship with its own first-party virtual devices instead of depending on third-party virtual cables.

## macOS Implementation

### Phase 1: Use existing virtual device

Use an open-source virtual audio driver whose license is compatible with future commercial use.

Default candidates:

- Roc VAD: macOS virtual audio device under MPL-2.0. It is designed for real-time streaming to and from virtual devices, supports macOS 10.15+, and explicitly documents vendoring/forking requirements such as changing bundle identifiers.[13]
- Soundflower: MIT license and historically useful for macOS audio loopback, but it is an older system extension and should be treated as a compatibility spike rather than the modern default.[14]

Not default:

- BlackHole: strong technical candidate, but GPL-3.0. Its README states integration is allowed when the app is GPL-3.0; otherwise the author should be contacted for other applications. That makes it unsuitable as the default dependency for a proprietary commercial build.[15]
- Background Music: GPL-2.0, useful reference for macOS audio routing concepts but not a default proprietary dependency.[16]

audioBlade provides:

- Route persistence
- Profile switching
- Device reconnect handling
- GUI routing matrix

### Phase 2: Native Core Audio virtual device

Build a Core Audio HAL AudioServerPlugIn-style virtual device, or evaluate current DriverKit requirements depending on target macOS versions.[3][4]

macOS considerations:

- Virtual audio devices are not just normal app code
- Installation may require user approval
- Entitlements, notarization, and packaging matter.[7]
- Multi-output routing requires clock drift handling

## Network / TuneBlade-like Streaming

If TuneBlade parity specifically means streaming Windows audio to AirPlay receivers, treat it as an optional sink type. TuneBlade itself is positioned as Windows software for streaming audio to AirPlay receivers, so this is related but distinct from the Studio One-style local patch-bay use case.[8]

```text
Audio source -> audioBlade engine -> encoder -> AirPlay/network sink
```

Potential roadmap:

- Local device routing first
- RTP/RAOP/AirPlay research spike second
- Receiver discovery via mDNS/Bonjour
- Latency compensation and sync later

Network streaming should not be the first milestone unless AirPlay is the primary requirement, because the virtual-device and routing foundation is still needed.

## MVP Definition

MVP should prove the product solves the Studio One replacement use case.

MVP capabilities:

- Runs on Windows first, macOS second
- Starts at login as a background engine
- Lists physical and virtual audio devices
- Creates simple stereo routes
- Saves and restores one profile
- Handles device disconnect/reconnect
- Provides a small GUI route matrix
- Uses existing virtual audio devices instead of shipping a custom driver

MVP non-goals:

- First-party virtual driver
- AirPlay streaming
- Per-application routing
- Surround routing
- Advanced mixer effects
- Sample-accurate synchronization across many devices

## Development Phases

### Phase 0: Prototype and measurement

- Create JUCE app skeleton
- Enumerate devices on both OSes
- Implement route config schema
- Test loopback via existing virtual cable
- Measure latency, CPU usage, glitch behavior

### Phase 1: Persistent stereo router

- Background engine
- GUI route editor
- Device persistence by stable IDs
- Auto-start
- One-click profile restore
- Basic logging and diagnostics

### Phase 2: Production router

- Multi-channel routing
- Per-route gain/mute
- Sample-rate conversion
- Drift correction
- WASAPI exclusive mode
- ASIO support
- macOS Core Audio tuning

### Phase 3: First-party virtual devices

- Windows virtual audio driver
- macOS virtual audio plugin/driver
- Signed installers
- Notarization and driver approval flow

### Phase 4: Network sinks

- AirPlay-like output
- mDNS discovery
- Multi-receiver sync
- Latency calibration

## Key Technical Risks

- Virtual device implementation is driver-level work, not normal app code
- Windows driver signing and macOS notarization can dominate schedule
- Routing between multiple hardware devices needs clock drift correction
- Low latency and glitch-free operation require native real-time code
- Per-application routing differs significantly by platform
- AirPlay compatibility can be protocol and licensing sensitive

## Initial Repository Direction

Recommended first codebase shape:

```text
audioBlade/
  CMakeLists.txt
  apps/
    desktop/
  control/
    service/
    ipc/
    profiles/
  engine/
    audio_graph/
    devices/
    routing/
  platform/
    windows/
    macos/
  docs/
  tools/
```

Start with a JUCE + CMake prototype to validate audio routing quickly. Keep the control-plane code separate from the audio engine so that a Rust service can be introduced later without rewriting real-time routing logic.

## First Concrete Milestone

Build a desktop prototype that can:

1. List available audio input/output devices.
2. Let the user select one input and one output.
3. Pass stereo audio from input to output continuously.
4. Save that route to a profile file.
5. Relaunch and restore the route automatically.

This validates the core routing layer before investing in custom virtual drivers.

## References

1. Microsoft Learn: Loopback Recording. https://learn.microsoft.com/en-us/windows/win32/coreaudio/loopback-recording
2. Microsoft Windows Driver Samples: SYSVAD Virtual Audio Device Driver Sample. https://learn.microsoft.com/en-us/samples/microsoft/windows-driver-samples/sysvad-virtual-audio-device-driver-sample/
3. Apple Developer Documentation: AudioDriverKit. https://developer.apple.com/documentation/audiodriverkit
4. Apple Developer Documentation: Core Audio. https://developer.apple.com/documentation/coreaudio
5. JUCE Documentation: Audio device management and cross-platform audio framework. https://juce.com/learn/documentation/
6. Microsoft Learn: Driver signing. https://learn.microsoft.com/en-us/windows-hardware/drivers/install/driver-signing
7. Apple Developer Documentation: Notarizing macOS software before distribution. https://developer.apple.com/documentation/security/notarizing_macos_software_before_distribution
8. TuneBlade official website. https://www.tuneblade.com/
9. VirtualDrivers/Virtual-Audio-Driver GitHub repository. https://github.com/VirtualDrivers/Virtual-Audio-Driver
10. VirtualDrivers/Virtual-Audio-Driver LICENSE. https://raw.githubusercontent.com/VirtualDrivers/Virtual-Audio-Driver/main/LICENSE
11. Microsoft Windows Driver Samples LICENSE, Microsoft Public License. https://raw.githubusercontent.com/microsoft/Windows-driver-samples/main/LICENSE
12. VB-Audio CABLE license/store page. https://vb-audio.com/Cable
13. Roc VAD GitHub repository, MPL-2.0 macOS virtual audio device. https://github.com/roc-streaming/roc-vad
14. Soundflower GitHub repository, MIT-licensed macOS audio extension. https://github.com/mattingalls/Soundflower
15. BlackHole GitHub repository and GPL-3.0 integration note. https://github.com/ExistentialAudio/BlackHole
16. Background Music GitHub repository. https://github.com/kyleneideck/BackgroundMusic
17. VoiceMeeter official overview. https://voicemeeter.com/see-what-voicemeeter-can-do-overview/
18. VoiceMeeter virtual inputs and outputs documentation. https://voicemeeter.com/quick-tips-voicemeeter-virtual-inputs-and-outputs-windows-10-and-up/
