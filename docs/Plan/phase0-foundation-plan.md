# Phase 0 Foundation Development Plan

## Purpose

This plan is the canonical English plan for the first audioBlade Harness development round.

The goal is not to build the full audio router yet. The goal is to deliver the first verifiable foundation module:

> A buildable C++/JUCE desktop application skeleton that can enumerate system audio devices, emit reproducible diagnostics, and establish the base model needed for later route/profile persistence.

This foundation is the prerequisite for the Windows route engine, virtual audio endpoints, profile restore, and background lifecycle work.

## Timeline

- Start date: 2026-06-23
- End date: 2026-07-20
- Duration: 4 weeks
- Cadence: one verifiable increment per week, with verification notes completed before the end of each week

## Scope

### Included

- CMake + JUCE project skeleton
- Minimal desktop application entry point
- Base repository directory structure
- Windows-first audio device enumeration
- Device diagnostics output
- Initial Route/Profile schema
- Minimal automated verification commands
- ADR for first-round technical decisions

### Excluded

- Virtual audio driver integration
- First-party driver development
- AirPlay or network output
- Real audio forwarding
- Background service split
- ASIO support
- Effects processing
- Full route matrix UI
- Per-application routing

## Target Deliverables

By the end of this phase, the repository should contain:

- `CMakeLists.txt`
- `apps/desktop/` minimal JUCE application
- `engine/devices/` device enumeration abstraction
- `control/profiles/` initial Profile schema
- `tools/diagnostics/` device diagnostics entry point
- `docs/adr/` with at least one ADR
- Runnable build and diagnostics verification commands

## Success Criteria

This phase is complete only when the project can demonstrate:

- The project can configure and build in a Windows development environment.
- The desktop app can launch to a minimal window or status surface.
- The app or diagnostics tool can list audio input/output devices.
- Enumeration output includes at least device name, direction, channel count, and sample-rate or available-format information when available.
- Diagnostics output can be saved as text or JSON for later bug reproduction.
- The Profile schema can represent a future stereo route with source, destination, gain, mute, and sample-rate policy.
- Verification steps are recorded with commands or manual evidence.

## Weekly Plan

### Week 1: Project Skeleton And Build Baseline

Dates: 2026-06-23 to 2026-06-29

Goal: turn the documentation-first repository into a buildable engineering repository.

Deliverables:

- Root `CMakeLists.txt`
- `apps/desktop/` minimal JUCE application structure
- Base `engine/`, `control/`, `platform/`, and `tools/` directories
- `docs/adr/0001-mvp-stack-and-boundaries.md`
- Minimal build commands in README or developer docs

Tasks:

- Decide how JUCE is introduced: Git submodule, FetchContent, system path, or package manager.
- Establish CMake target naming.
- Create the minimal desktop app entry point.
- Add basic version and application name metadata.
- Write the ADR explaining why Phase 0 uses a C++/JUCE single-process path and defers the Rust control service and virtual drivers.

Acceptance criteria:

- CMake configure succeeds.
- The build system can be generated.
- The minimal app target exists.
- The ADR explains why Rust service split and virtual driver integration are deferred.

Verification commands or evidence:

```bash
cmake -S . -B build
cmake --build build
test -f docs/adr/0001-mvp-stack-and-boundaries.md
```

Risks:

- JUCE acquisition may be unstable in the mixed Windows/WSL setup.
- The current Linux workspace is not the real Windows audio development environment.
- Over-designing the directory structure could prevent a buildable Week 1 result.

Mitigation:

- Week 1 only needs a build skeleton, not real audio functionality.
- Windows-only validation should be documented as an environment requirement rather than falsely completed from WSL.

### Week 2: Device Enumeration

Dates: 2026-06-30 to 2026-07-06

Goal: implement the first visible audio capability: system input/output device enumeration.

Deliverables:

- `engine/devices/AudioDeviceInfo` data structure
- Initial `engine/devices/DeviceEnumerator`
- JUCE `AudioDeviceManager` integration
- Device list displayed in UI or logs
- Example device diagnostics output

Tasks:

- Define device fields: name, direction, channel count, default status, and sample-rate or format capability.
- Enumerate current system audio devices through JUCE.
- Emit enumeration results outside the real-time audio path.
- Distinguish input, output, duplex, or unknown device types.
- Document the Windows-first validation path.

Acceptance criteria:

- The app can list visible audio devices after launch.
- Inputs and outputs are distinguishable.
- Enumeration logic does not depend on Python.
- Enumeration logic is not placed inside a real-time audio callback.

Verification commands or evidence:

```bash
cmake --build build
./build/apps/desktop/audioBlade --diagnose-devices
```

Manual evidence:

- Save one device enumeration output under `docs/evidence/`.
- Record OS, audio interface, default input, and default output.

Risks:

- JUCE may expose different fields across platforms.
- Windows audio device names and IDs may change after reboot or reconnect.
- The current development environment may not expose real Windows audio devices.

Mitigation:

- Define a minimal cross-platform field set first, then put platform-specific fields in a diagnostics extension area.
- Treat WSL as build/static-check environment only, not audio device acceptance environment.

### Week 3: Profile Schema And Diagnostics Output

Dates: 2026-07-07 to 2026-07-13

Goal: establish the configuration foundation for later persistent routing.

Deliverables:

- `control/profiles/profile.schema.json`
- `control/profiles/examples/default-monitor-route.json`
- `tools/diagnostics/` diagnostics entry point
- `docs/adr/0002-profile-schema-v0.md`
- Profile schema validation notes

Tasks:

- Define minimal JSON representations for `AudioNode`, `Port`, `Route`, and `Profile`.
- Support one stereo route with source, destination, gain, mute, and `sampleRatePolicy`.
- Reserve fields for stable device IDs and human-readable fallback matching.
- Make device enumeration diagnostics compatible with future Profile device references.
- Document required v0 fields and future-only fields.

Acceptance criteria:

- The schema can represent a future virtual-input-to-physical-output route.
- The schema can represent a future physical-input-to-virtual-output route.
- The example Profile validates against the schema.
- Documentation states clearly that v0 defines a serializable model only and does not perform real routing.

Verification commands or evidence:

```bash
test -f control/profiles/profile.schema.json
test -f control/profiles/examples/default-monitor-route.json
python3 -m json.tool control/profiles/examples/default-monitor-route.json >/dev/null
```

If a schema validation tool is introduced, add:

```bash
python3 tools/diagnostics/validate_profile.py control/profiles/examples/default-monitor-route.json
```

Risks:

- The schema may become too complex too early.
- Profile references may bind too tightly to unstable device IDs.
- Future multichannel, network sink, or effects concepts may creep into v0.

Mitigation:

- v0 covers only stereo routes and required diagnostics fields.
- Multichannel, network, and effects concepts stay as future extensions, not Phase 0 acceptance criteria.

### Week 4: Integration, Verification, And Next Phase Entry

Dates: 2026-07-14 to 2026-07-20

Goal: integrate the skeleton, device enumeration, diagnostics output, and Profile schema into a deliverable foundation module.

Deliverables:

- `docs/Plan/phase0-verification-report.md`
- `docs/evidence/` with at least one device enumeration sample
- Build and diagnostics command list
- Next-phase recommendation
- Known risks list

Tasks:

- Clean up build instructions.
- Document diagnostics output format.
- Run one real device enumeration in a Windows environment.
- In WSL/Linux, run only build or static checks and do not claim audio device acceptance.
- Check scope against `AGENTS.md`, `docs/Harness/agents.md`, and `docs/cn/长期开发计划.md`.
- Confirm whether the next phase should enter a local input-to-output stereo route prototype.

Acceptance criteria:

- A new developer can configure, build, and run diagnostics from docs.
- The verification report includes command evidence, output summary, and not-run items.
- Phase 0 does not introduce virtual drivers, first-party drivers, AirPlay, or real routing claims.
- The next phase entry point is explicit.

Verification commands or evidence:

```bash
cmake -S . -B build
cmake --build build
./build/apps/desktop/audioBlade --diagnose-devices
python3 -m json.tool control/profiles/examples/default-monitor-route.json >/dev/null
rg -n "AirPlay|first-party|VoiceMeeter|ASIO" docs/Plan/phase0-verification-report.md
```

Risks:

- Real Windows validation may not happen in time.
- Device enumeration output format may change frequently.
- The foundation module may drift into real routing implementation.

Mitigation:

- The verification report may mark items as `not_run`, but must explain why.
- Diagnostics output format should be compatible after v0.
- Real audio forwarding remains the next phase, not Phase 0.

## Role Responsibilities

### Planner

- Maintain cadence and scope.
- Re-check weekly whether the work still serves the first verifiable foundation module.
- Prevent early expansion into virtual drivers or network output.

### Implementer

- Build the CMake/JUCE skeleton.
- Implement device enumeration and diagnostics output.
- Create Profile schema and examples.

### Reviewer

- Check real-time audio boundaries.
- Check for premature virtual driver, ASIO, AirPlay, or effects work.
- Check documentation and implementation consistency.

### Verifier

- Run build checks.
- Run device enumeration checks.
- Validate JSON examples.
- Write the verification report.

## Minimum Definition Of Done

Phase 0 is complete only when all of the following are true:

- The repository can build a minimal desktop application.
- Device enumeration diagnostics can run.
- There is real device enumeration evidence, or a clearly marked limitation explaining why not.
- Profile schema and example files exist and can be validated.
- ADRs record key Phase 0 technical choices.
- The verification report lists passed, failed, and not-run items.

## Next Phase Entry

After this plan is complete, the next phase should be:

> Windows local stereo route prototype: choose one input device and one output device, continuously forward stereo audio, and record latency, CPU, and glitch behavior.

Still out of scope:

- Virtual driver integration
- First-party driver development
- AirPlay or network sink
- Full route matrix UI
