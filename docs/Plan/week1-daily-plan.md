# Phase 0 Week 1 Daily Plan

## Purpose

This is the daily execution plan for Phase 0 Week 1. It expands the Week 1 section of `docs/Plan/phase0-foundation-plan.md`.

Week 1 goal:

> Turn the documentation-first repository into a buildable engineering repository without claiming real audio routing, virtual driver integration, or Windows audio acceptance.

## Week 1 Dates

- Start: 2026-06-23
- End: 2026-06-29

## Decisions For Week 1

- JUCE acquisition strategy: use a pinned source checkout or Git submodule under `extern/JUCE`.
- CMake is the source of truth for native C++/JUCE builds.
- Phase 0 starts as a single-process C++/JUCE desktop application path.
- Target naming:
  - CMake project: `audioBlade`
  - Desktop app target: `audioBladeDesktop`
  - Executable output name, once implemented: `audioBlade`
  - Future native libraries: `audioBladeEngine`, `audioBladeControl`, `audioBladePlatform`
- Application metadata:
  - App name: `audioBlade`
  - Version: `0.1.0`
  - Bundle/company identifier placeholder: `dev.audioblade`

## Daily Plan

### Day 1: Build Baseline Decisions

Date: 2026-06-23

Tasks:

- Confirm JUCE acquisition method.
- Define CMake ownership, target naming, and application metadata.
- Create the first ADR for MVP stack and boundaries.
- Add a minimal configure-only CMake skeleton that fixes project metadata and target names.
- Add developer build notes.

Acceptance criteria:

- JUCE acquisition strategy is explicit.
- CMake target naming is fixed.
- `docs/adr/0001-mvp-stack-and-boundaries.md` exists.
- CMake configure succeeds in the current WSL environment.
- Real routing, virtual drivers, ASIO, AirPlay, effects, per-app routing, and background service split remain explicitly deferred.

### Day 2: Repository Skeleton

Date: 2026-06-24

Tasks:

- Expand source directories only where concrete files are needed.
- Add `apps/desktop/` CMake entry and minimal source stub if JUCE is available.
- Keep `engine/`, `control/`, `platform/`, and `tools/` aligned with the architecture split.

Acceptance criteria:

- Directory layout supports the long-term architecture without overfilling empty modules.
- CMake can discover the desktop app target.

### Day 3: Minimal Desktop App Entry

Date: 2026-06-25

Tasks:

- Add a minimal JUCE app entry point.
- Wire app metadata into CMake/JUCE.
- Keep audio device enumeration outside this day unless it is a trivial non-blocking diagnostic.

Acceptance criteria:

- `audioBladeDesktop` target is implementable with pinned JUCE sources.
- The source layout keeps GUI, future control code, and future engine code separated.

### Day 4: ADR Completion

Date: 2026-06-26

Tasks:

- Review and complete ADR 0001.
- Record consequences, non-goals, and deferrals.
- Confirm the ADR matches `docs/DEVELOPMENT_PROCESS.md` and `docs/DEVELOPMENT_GUIDELINES.md`.

Acceptance criteria:

- ADR states why C++/JUCE is first.
- ADR states why Rust control service is deferred.
- ADR states why virtual driver integration and real routing are deferred.

### Day 5: Developer Commands

Date: 2026-06-27

Tasks:

- Ensure root README or developer docs include configure/build commands.
- Add notes on WSL/Linux limitations.
- Document how to provide JUCE sources.

Acceptance criteria:

- A developer can find the current configure command.
- Docs do not claim Windows audio validation from WSL.

### Day 6: Verification Pass

Date: 2026-06-28

Tasks:

- Run CMake configure.
- Run build if a concrete build target exists.
- Check ADR and target naming.
- Record blockers as `fail` or `not-run`.

Acceptance criteria:

- Configure succeeds, or failure is documented with a concrete cause.
- Any unrun checks are explicitly classified.

### Day 7: Week 1 Closeout

Date: 2026-06-29

Tasks:

- Write Week 1 status.
- List completed deliverables and not-run checks.
- Confirm Week 2 entry point: device enumeration.

Acceptance criteria:

- Week 1 deliverables are present or blockers are documented.
- Next work is clearly Week 2 device enumeration, not routing or drivers.

## Day 1 Not-Run Items

- Windows audio device validation from WSL.
- JUCE GUI launch without pinned JUCE sources and a desktop target environment.
- Device enumeration diagnostics.
- Virtual driver checks.
- Real-time audio callback checks.
- Route persistence or profile restore checks.
