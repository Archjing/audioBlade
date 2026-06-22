# AGENTS.md

## Project Context

audioBlade is a planned Windows + macOS desktop audio routing application. Its goal is DAW-independent, persistent system audio routing with virtual audio endpoint support. It should let users route audio between physical audio interfaces, system/app playback, app or device inputs, and virtual inputs/outputs without keeping Studio One, Audition, or another DAW open.

This repository is currently documentation-first. Treat the existing docs as product and engineering source of truth before adding code.

Primary references:

- `docs/TECHNICAL_PLAN.md`: technical rationale, platform choices, driver references, and long-term architecture.
- `docs/DEVELOPMENT_GUIDELINES.md`: project-level engineering rules.
- `docs/DEVELOPMENT_PROCESS.md`: polyglot development process and toolchain ownership.
- `docs/cn/技术方案.md`: Chinese technical plan.
- `docs/cn/开发规范.md`: Chinese development guidelines.
- `docs/cn/长期开发计划.md`: long-term development plan.
- `docs/Plan/phase0-foundation-plan.md`: canonical English Phase 0 foundation plan.
- `docs/Harness/agents.md`: audioBlade-specific Harness Engineering roles and invocation rules.
- `docs/Harness/workflow.md`: project-level Harness workflow, task types, and artifact rules.
- `docs/Harness/team.md`: active Harness team charter, role responsibilities, and escalation rules.

## Harness Engineering Context

This project will be developed with a Harness Engineering workflow. When the user asks for role-based work such as "让 planner 写一个计划", "让架构 agent 看一下", "让实时音频/驱动 agent 审一下", or "让 verifier 做验证计划", read `docs/Harness/agents.md` and apply those role instructions before acting.

When the user asks for Harness-mode work such as compare, admission, document revision, experiment report, or plan review, read `docs/Harness/workflow.md` first and follow its artifact rules. When the user asks to create, use, or coordinate the Harness team, read `docs/Harness/team.md`.

The local harness runner profile is:

```text
/home/zj/workspace/codex-harness-runner/profiles/audioBlade.toml
```

If launching the harness is not necessary or would be too heavy, emulate the requested role directly using the constraints in `docs/Harness/agents.md` and `docs/Harness/workflow.md`, then write durable plans or decisions into repo-local docs. Planning documents should be written in English under `docs/Plan/`; Chinese translations belong under `docs/cn/Plan/`.

Development process questions, including whether to use `uv`, CMake, Cargo, or wrapper scripts, should follow `docs/DEVELOPMENT_PROCESS.md`.

## Product Boundary

audioBlade must provide:

- Persistent system-level routing without requiring a DAW.
- Routing between virtual audio endpoints and physical audio devices.
- Saved profiles that restore routes after restart, login, or device reconnect.
- A background audio engine independent of the GUI.
- A UI for editing routes, gains, mute state, device status, and profiles.

audioBlade must not depend on a DAW, editor, or foreground-only process to keep routes alive.

## Architecture Rules

Use this long-term split:

- `engine`: C++/JUCE real-time audio routing, graph state, device I/O, and routing callbacks.
- `control`: Rust control service for route graph ownership, profiles, device policy, diagnostics, and lifecycle, after the MVP audio path is proven.
- `app/gui`: route editor, status UI, profile UI, and diagnostics UI.
- `platform/windows`: WASAPI, ASIO, Windows virtual driver integration, and installer details.
- `platform/macos`: Core Audio, AudioDriverKit/HAL integration, and macOS packaging details.
- `drivers`: third-party driver integration notes and first-party driver experiments.
- `tools`: diagnostics, measurement scripts, and test utilities.
- `docs`: technical plans, ADRs, references, and risk notes.

For the first MVP, it is acceptable to keep control logic inside a JUCE app to reduce moving parts. Still keep control-plane and audio-plane responsibilities separated in code so a Rust control service can be split out later.

## Runtime And Language Rules

- Use C++ for real-time audio engine code.
- Use JUCE for initial cross-platform audio device management and the first MVP GUI.
- Use CMake for build orchestration.
- Use Rust later for non-real-time control services only.
- Use Python only for tooling, diagnostics, scripts, and tests.
- Do not put real-time audio callback code in Python.
- Do not rely on Python to expose OS-level virtual audio devices.
- Do not pass normal real-time audio buffers through Rust, IPC, or the GUI.

The normal audio buffer path should stay inside the native engine.

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

- Sample-rate mismatch.
- Buffer-size mismatch.
- Multi-device clock drift.
- Device disconnects.
- Device ID changes after reboot or reconnect.
- Windows shared/exclusive mode differences.

## Routing Model

Use a graph model, not hard-coded device pairs.

Required concepts:

- `AudioNode`: physical device, virtual device, app capture stream, or network sink.
- `Port`: input or output channel on a node.
- `Route`: connection from source ports to destination ports.
- `Profile`: named route set with gains, mutes, channel maps, and sample-rate policy.

All routes must be serializable. Profiles should prefer stable device identifiers where available and fall back to human-readable matching when IDs change.

## Driver Dependency Policy

Default virtual audio driver dependencies must be open source and compatible with future commercial use.

Preferred licenses:

- MIT
- BSD
- Apache-2.0
- MS-PL
- MPL-2.0

Allowed only after explicit review:

- LGPL
- File-level copyleft where obligations are isolated and manageable.

Do not use these as default dependencies for a proprietary commercial build:

- GPL-2.0 drivers
- GPL-3.0 drivers
- Closed-source freeware
- Donationware drivers
- Proprietary virtual cable products

Before bundling any driver in a commercial installer, run a formal license review. Repository docs are engineering guidance, not legal advice.

## Starting Driver Decisions

Windows:

- Start with `VirtualDrivers/Virtual-Audio-Driver`.
- Treat VoiceMeeter as a feature benchmark, not a dependency.
- Do not use VB-CABLE or VoiceMeeter as default dependencies.
- Do not start from raw SYSVAD unless the task is specifically first-party Windows driver R&D.

macOS:

- Start with `roc-streaming/roc-vad` as the first validation candidate.
- Treat Soundflower only as a compatibility reference.
- Treat BlackHole as a feature benchmark, not a default dependency.
- Do not bundle BlackHole into a proprietary build unless audioBlade becomes GPL-3.0-compatible or obtains a separate commercial license.

## MVP Order

Follow this order unless the user explicitly changes project strategy:

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

Treat Windows and macOS drivers as separate projects. Do not assume a shared cross-platform driver layer exists.

## Documentation Rules

- Record every major technical decision in `docs/TECHNICAL_PLAN.md` or a dedicated ADR-style document.
- Driver-related decisions must include repository URL, license URL, commercial-use assessment, functional gap assessment, packaging risks, and status.
- Keep Chinese docs aligned when changing user-facing planning or project guidance.
- Use plain language. Avoid ambiguous terms such as "spike" unless defined in the document.

## Testing And Verification

When adding code, prefer focused tests and reproducible diagnostics over broad unverified claims.

Minimum verification for audio work should record:

- OS version.
- Device names.
- Input/output channel counts.
- Sample rate.
- Buffer size.
- Audio API mode.
- Virtual driver version or commit when applicable.
- Steps to reproduce glitches, disconnects, or restore failures.

For long-running routing changes, test at least:

- Continuous stereo routing.
- Device reconnect behavior.
- Profile reload behavior.
- Sample-rate mismatch behavior when relevant.

## Scope Control

MVP includes:

- Device listing.
- Simple stereo routes.
- Route persistence.
- Background engine.
- Basic GUI.
- Device reconnect handling.
- License-compatible virtual driver dependencies.

MVP excludes:

- AirPlay streaming.
- Per-application routing.
- First-party virtual drivers.
- Surround routing.
- Effects processing.
- Full VoiceMeeter replacement features.
- Multi-receiver network sync.
