# ADR 0001: MVP Stack And Phase 0 Boundaries

## Status

Accepted for Phase 0.

## Date

2026-06-23

## Context

audioBlade is a Windows and macOS desktop audio routing application. The long-term product needs persistent routing, virtual endpoint support, saved profiles, device reconnect behavior, and a GUI that does not require a DAW to stay open.

Phase 0 is not the full router. It establishes the first verifiable foundation: repository structure, native build ownership, a minimal desktop application path, diagnostics direction, and the route/profile model needed by later phases.

## Decision

Phase 0 starts with a C++/JUCE single-process desktop application path.

Build ownership:

- CMake owns native C++/JUCE builds.
- JUCE will be provided as a pinned source checkout or Git submodule under `extern/JUCE`.
- Python remains tooling-only.
- Rust is deferred until the non-real-time control service has enough responsibility to justify a process or language boundary.

Initial CMake naming:

- Project: `audioBlade`
- Desktop app target: `audioBladeDesktop`
- Future executable output name: `audioBlade`
- Future native libraries: `audioBladeEngine`, `audioBladeControl`, `audioBladePlatform`

Initial application metadata:

- App name: `audioBlade`
- Version: `0.1.0`
- Identifier placeholder: `dev.audioblade`

## Rationale

C++/JUCE is the shortest path to a cross-platform native desktop shell and later audio device enumeration. It also keeps the future real-time audio path in native code.

A single-process Phase 0 path reduces moving parts while the basic app, build, diagnostics, and profile model are still being proven. The code and documentation should still preserve the future split between GUI, control-plane logic, and audio engine responsibilities.

Rust is useful for a later control service, but adding it before the audio path and profile model are proven would increase integration complexity without improving Phase 0 verification.

Virtual driver integration is essential later, but Day 1 only documents candidate direction. It does not bundle, load, sign, install, or validate any Windows or macOS virtual audio driver.

## Consequences

Positive:

- Phase 0 has a clear native build owner.
- The project can move from documentation-only planning toward verifiable build structure.
- Future real-time audio code remains outside Python, Rust IPC, and GUI-only paths.

Tradeoffs:

- A pinned JUCE checkout or submodule must be provided before a real desktop target can build.
- The first CMake skeleton may configure before the desktop app target has full source implementation.
- WSL/Linux checks can prove repository and build-system integrity only; they do not prove Windows audio behavior.

## Explicit Non-Goals

This ADR does not admit:

- Real audio routing or forwarding.
- Virtual audio driver integration.
- First-party driver development.
- ASIO support.
- AirPlay or network output.
- Effects processing.
- Per-application routing.
- Background service split.
- Rust control service implementation.
- Claims of glitch-free, low-latency, lock-free, allocation-free, or real-time-safe routing.

## Verification

Day 1 verification is limited to:

- File and directory presence.
- CMake configure generation.
- Target naming and metadata decisions.
- ADR coverage of stack and boundary decisions.

Driver, routing, device enumeration, GUI launch, latency, and real-time callback behavior must be marked `not-run` unless validated later on the required host environment with recorded evidence.
