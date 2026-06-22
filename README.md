# audioBlade

[![English](https://img.shields.io/badge/lang-English-blue.svg)](./README.md)
[![简体中文](https://img.shields.io/badge/lang-简体中文-red.svg)](./README.zh-CN.md)

audioBlade is a planned Windows + macOS desktop audio routing application for DAW-independent, persistent system audio routing with virtual endpoint support.

The repository is currently in Phase 0: foundation work.

## Current Phase

Phase 0 establishes:

- CMake ownership for native C++/JUCE builds.
- A minimal desktop application target name.
- Base architecture directories.
- ADRs and developer documentation.
- Later device diagnostics and profile schema work.

Phase 0 Day 1 does not implement real audio routing, virtual audio drivers, ASIO, AirPlay/network output, effects, per-application routing, or a background service split.

## JUCE Source Strategy

JUCE should be provided as a pinned source checkout or Git submodule at:

```text
extern/JUCE
```

The current CMake skeleton configures and builds a placeholder `audioBladeDesktop` target when `extern/JUCE` is not present. That placeholder proves build-system structure only; it does not launch a GUI or validate audio behavior.

## Configure And Build

From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

Preset form:

```bash
cmake --preset dev
cmake --build --preset dev
```

## WSL Limitations

WSL/Linux checks can verify repository structure, CMake configuration, and placeholder build behavior. They do not prove:

- Windows WASAPI behavior.
- macOS Core Audio behavior.
- Physical audio device visibility.
- Virtual endpoint support.
- GUI launch quality.
- Real-time callback safety.
- Audio routing correctness.

Those checks must be recorded later as `not-run`, `pass`, or `fail` with host OS evidence.

## Project Documents

- `AGENTS.md`: project instructions and architecture rules.
- `docs/TECHNICAL_PLAN.md`: technical plan and rationale ([Simplified Chinese](./docs/cn/技术方案.md)).
- `docs/DEVELOPMENT_GUIDELINES.md`: engineering rules.
- `docs/DEVELOPMENT_PROCESS.md`: toolchain ownership.
- `docs/Plan/`: planning documents.
- `docs/Harness/`: Harness workflow and team charter.
- `docs/adr/`: architecture decisions.
