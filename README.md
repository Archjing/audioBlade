# audioBlade 音刃

[![English](https://img.shields.io/badge/lang-English-blue.svg)](./README.md)
[![简体中文](https://img.shields.io/badge/lang-简体中文-red.svg)](./README.zh-CN.md)

audioBlade is a planned Windows + macOS desktop audio routing application for DAW-independent, persistent system audio routing with virtual endpoint support.

The repository is currently in Phase 0: foundation work. Week 3 is complete, and the project is entering Week 4 verification closeout.

## Current Phase

Phase 0 has established:

- CMake ownership for native C++/JUCE builds.
- A minimal desktop application target.
- Base architecture directories.
- Windows-first device enumeration through JUCE.
- Device diagnostics output with backend, direction, channel count, and OS metadata.
- Profile schema v0 for future persistent stereo routes.
- ADRs, Harness workflow documents, evidence, and closeout reports.

Current status:

- Week 1: project skeleton and build baseline complete.
- Week 2: Windows device enumeration complete with recorded evidence.
- Week 3: Profile schema and diagnostics compatibility complete.
- Week 4: verification report, integration summary, risk list, and next-phase recommendation are next.

Phase 0 still does not implement real audio routing, virtual audio drivers, ASIO, AirPlay/network output, effects, per-application routing, profile restore, or a background service split.

## JUCE Source Strategy

JUCE should be provided as a pinned source checkout or Git submodule at:

```text
extern/JUCE
```

The current desktop target is `audioBladeDesktop`. With `extern/JUCE` present, it builds the JUCE desktop application and supports the Phase 0 diagnostics argument.

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

## Device Diagnostics

On a Windows host build:

```powershell
.\build\apps\desktop\audioBladeDesktop_artefacts\Debug\audioBlade.exe --diagnose-devices
```

Week 2 evidence is recorded in:

```text
docs/evidence/week2-device-enumeration-sample.md
```

## Profile Validation

Profile schema v0 and its example live under:

```text
control/profiles/
```

Validate the example profile:

```bash
python3 -m json.tool control/profiles/profile.schema.json >/dev/null
python3 -m json.tool control/profiles/examples/default-monitor-route.json >/dev/null
python3 tools/diagnostics/validate_profile.py control/profiles/examples/default-monitor-route.json
```

## WSL Limitations

WSL/Linux checks can verify repository structure, CMake configuration, JSON/profile validation, and some build behavior. They do not prove:

- Windows WASAPI behavior.
- macOS Core Audio behavior.
- Physical audio device visibility.
- Virtual endpoint support.
- GUI launch quality.
- Real-time callback safety.
- Audio routing correctness.

Current WSL build note: a full JUCE audio-device build may require Linux audio development headers such as ALSA. Windows host build and device-enumeration evidence remain the authoritative Week 2 audio-device validation path.

## Project Documents

[![Agents](https://img.shields.io/badge/docs-AGENTS-2f6fbb.svg)](./AGENTS.md)
[![Technical Plan](https://img.shields.io/badge/docs-Technical%20Plan-2f6fbb.svg)](./docs/TECHNICAL_PLAN.md)
[![Technical Plan CN](https://img.shields.io/badge/docs-%E6%8A%80%E6%9C%AF%E6%96%B9%E6%A1%88-red.svg)](./docs/cn/技术方案.md)
[![Guidelines](https://img.shields.io/badge/docs-Guidelines-2f6fbb.svg)](./docs/DEVELOPMENT_GUIDELINES.md)
[![Process](https://img.shields.io/badge/docs-Process-2f6fbb.svg)](./docs/DEVELOPMENT_PROCESS.md)
[![Plans](https://img.shields.io/badge/docs-Plans-2f6fbb.svg)](./docs/Plan/)
[![Harness](https://img.shields.io/badge/docs-Harness-2f6fbb.svg)](./docs/Harness/)
[![ADR](https://img.shields.io/badge/docs-ADR-2f6fbb.svg)](./docs/adr/)
