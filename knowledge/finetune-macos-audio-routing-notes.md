# FineTune macOS Audio Routing Notes

Source repository: https://github.com/ronitsingh10/FineTune  
Ingested snapshot: `71b78ad5c6cd38079b6cc7e8c01eb1ba889381aa`  
Snapshot commit date: 2026-06-14  
Ingested for: audioBlade future product and architecture reference  

## License Boundary

FineTune is licensed under GPLv3. Treat it as a product, architecture, testing, and workflow reference only.

Do not copy FineTune source code, tests, assets, or implementation text into audioBlade unless audioBlade deliberately adopts GPLv3-compatible licensing or receives separate permission from the author. The notes below are design observations and project learnings, not imported code.

## Why It Matters To audioBlade

FineTune is a macOS menu bar audio utility that covers several product areas relevant to audioBlade:

- Per-app volume and mute control.
- App-to-output-device routing.
- Multi-device output.
- Device priority and reconnect restore.
- Audio capture permission UX.
- Device inspector and diagnostics.
- EQ, AutoEQ headphone correction, loudness handling, and limiter concepts.
- URL scheme automation.
- macOS signing, notarization, DMG release, and Sparkle appcast flow.

FineTune is not a direct substitute for audioBlade's target architecture. It is macOS-only, targets macOS 15+, and relies heavily on Core Audio process taps and aggregate devices. audioBlade still needs cross-platform virtual endpoint support, persistent routing profiles, and eventually Windows/macOS virtual driver integration.

## Product Lessons

FineTune's user model suggests several useful audioBlade UX ideas:

- Keep a compact always-available control surface for daily routing changes.
- Let users pin inactive apps or routes so configuration can happen before audio starts.
- Provide an ignore/disable path for problematic apps or devices so users can quickly return them to normal OS routing.
- Make device priority explicit and persistent.
- Restore app/device routing after reconnect instead of treating reconnect as a new setup.
- Expose device UID, sample rate, transport type, and backend capability in an inspector.
- Offer automation through URL schemes or CLI-equivalent commands.
- Make permission failures visible and actionable rather than hiding them in logs.

For audioBlade, the equivalent UX should map to route profiles, route matrix entries, device fallback policy, and persistent virtual endpoint state.

## macOS Architecture Observations

FineTune uses several macOS-specific Core Audio patterns worth remembering:

- It monitors Core Audio device lists and process object lists with property listeners.
- It uses periodic refresh as a safety net because listener events can be missed during rapid lifecycle changes.
- It debounces device-list updates because HAL devices can be unstable during short connect/disconnect bursts.
- It tracks devices by stable UID and keeps O(1) UID and device-ID lookup maps.
- It distinguishes output devices, input devices, hidden devices, aggregate devices, and app-owned private aggregate devices.
- It uses per-app/process capture concepts and private aggregate devices for app routing.
- It treats Bluetooth transitions and sample-rate changes as routing events, not just UI updates.
- It keeps fallback and reconnect policy in the engine/control layer rather than leaving it to the view.

audioBlade should keep these as macOS backend research inputs. Process taps may become an optional macOS app-capture backend, but they should not replace the virtual audio endpoint layer required by audioBlade's cross-platform product goal.

## Real-Time Audio Lessons

FineTune's codebase is explicit about separating control work from real-time audio work:

- Setup, teardown, device switching, and state mutation happen on the main/control side.
- Audio processing runs on Core Audio's HAL I/O thread.
- Audio callbacks avoid allocation, locks, logging, Objective-C messaging, and file I/O.
- Audio-thread state is kept small and designed for lock-free reads.
- Volume changes are ramped to avoid clicks.
- Output gating and crossfade state machines are used to avoid startup and device-switch artifacts.
- Each active callback path needs its own DSP state when callbacks can overlap.
- Health checks wait until activation has settled and callbacks have actually run.

For audioBlade, this reinforces the existing rule: keep real-time audio buffers inside the native engine, and send only control messages across GUI/service boundaries.

## Device Lifecycle And Reconnect Policy

FineTune's approach suggests these durable concepts for audioBlade profiles:

- Device priority order should be user-editable and persisted.
- New devices should be registered without disrupting existing order.
- Device disconnect should trigger an explicit fallback decision.
- Device reconnect should try to restore intended routing where possible.
- There should be a distinction between "current fallback device" and "user's intended device."
- Stale or not-yet-alive devices may need watchers and timeout-based retries.
- Repeated tap or route recovery should have cooldowns to avoid restart loops.
- Bluetooth and display audio devices need special handling because sample rate, transport, and readiness can change after initial appearance.

audioBlade's profile schema should preserve both stable device IDs and human-readable fallback metadata.

## Settings And Profile Schema Lessons

FineTune uses a versioned JSON settings model with tolerant decoding. Useful schema ideas for audioBlade:

- Include a schema version from the first profile format.
- Decode missing fields with explicit defaults.
- Clamp invalid numeric values during load.
- Keep per-app, per-device, and global settings separated.
- Store user intent separately from computed runtime state.
- Use stable identifiers as primary keys, but keep display names for diagnostics and fallback matching.
- Treat hidden devices, ignored apps, pinned apps, device priority, and route mode as first-class state.
- Test decoding old settings fixtures whenever the schema changes.

audioBlade should adopt a profile migration policy before implementing profile persistence.

## Permission And Troubleshooting Lessons

FineTune documents permission paths clearly:

- Screen & System Audio Recording permission for app audio capture.
- Microphone permission for input monitoring.
- Accessibility permission for media key interception.
- Relaunch or manual System Settings changes may be required after permission changes.

audioBlade's equivalent future UX should document:

- Virtual driver installation approval.
- macOS system extension, DriverKit, or HAL plugin approval if used.
- Windows driver signing and installation trust prompts.
- Microphone/system audio capture permissions where applicable.
- Clear failure states for missing device, permission denied, driver not loaded, and route restore failure.

## Testing Lessons

FineTune has focused tests around:

- Settings decoding and migration.
- Per-app volume and mute behavior.
- Device priority and device inspector behavior.
- Aggregate-device planning.
- Crossfade and output-gate state machines.
- Biquad/EQ math and adversarial DSP cases.
- Loudness processing.
- Media key lifecycle and permission edge cases.
- URL/shortcut behavior.

audioBlade should create comparable tests around:

- Route graph serialization and validation.
- Profile schema migration.
- Stable device matching and fallback matching.
- Device disconnect/reconnect state transitions.
- Gain, mute, and channel map invariants.
- Real-time-safe DSP utilities.
- Driver/backend capability detection.
- Automation command parsing.

## Release And Distribution Lessons

FineTune's GitHub workflows show a practical macOS release shape:

- CI builds and tests on macOS with code signing disabled for PR validation.
- Release builds import signing certificates from secrets.
- Release archives are exported, signed, packaged as DMG, notarized, stapled, and uploaded to GitHub Releases.
- Sparkle appcast is generated and committed as part of tagged release automation.

audioBlade will need a broader version of this:

- Windows signing and installer pipeline.
- macOS signing, notarization, and driver/system-extension approval flow.
- Separate verification for GUI app, engine service, and driver/backend components.
- Release notes that include driver compatibility and known device limitations.

## Ideas Worth Considering Later

Near term:

- Add a profile schema ADR before implementing persistent routes.
- Add a device identity note for UID, display name, transport, channel count, sample rate, and backend capability.
- Add a route automation surface design, either URL scheme, CLI, or local control API.
- Add a macOS permission and driver approval checklist.

Medium term:

- Research macOS process taps as an optional per-app capture backend for macOS 15+.
- Compare process-tap routing with virtual endpoint routing and document product tradeoffs.
- Prototype device reconnect policy as a pure state machine before binding it to JUCE/Core Audio.
- Add route health checks that distinguish "route configured" from "audio callback actually running."

Long term:

- Treat FineTune-style per-app controls as an optional layer above audioBlade's core virtual endpoint router.
- Keep EQ, AutoEQ, loudness, and limiter work out of Phase 0 unless they become necessary to protect routing quality.
- Revisit Sparkle or another updater only after app signing and driver distribution are better understood.

## Source Files Reviewed

Primary files reviewed from the FineTune snapshot:

- `README.md`
- `README.zh-CN.md`
- `CONTRIBUTING.md`
- `LICENSE`
- `guide/autoeq.md`
- `guide/troubleshooting.md`
- `guide/url-schemes.md`
- `.github/workflows/ci.yml`
- `.github/workflows/release.yml`
- `FineTune/Audio/Engine/AudioEngine.swift`
- `FineTune/Audio/Engine/ProcessTapController.swift`
- `FineTune/Audio/Engine/TapResources.swift`
- `FineTune/Audio/Engine/CrashGuard.swift`
- `FineTune/Audio/Monitors/AudioDeviceMonitor.swift`
- `FineTune/Audio/Monitors/AudioProcessMonitor.swift`
- `FineTune/Audio/Monitors/DeviceVolumeMonitor.swift`
- `FineTune/Audio/Permission/AudioRecordingPermission.swift`
- `FineTune/Settings/SettingsManager.swift`
- `FineTune/Utilities/URLHandler.swift`
- `FineTuneTests/*`

