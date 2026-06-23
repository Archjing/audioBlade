# Phase 0 Week 2 Daily Plan

## Purpose

This is the daily execution plan for Phase 0 Week 2. It expands the Week 2 section of `docs/Plan/phase0-foundation-plan.md`.

Week 2 goal:

> Implement the first visible audio capability: system input/output device enumeration, with diagnostics output kept outside the real-time audio path.

## Week 2 Dates

- Start: 2026-06-30
- End: 2026-07-06

## Decisions For Week 2

- Week 2 execution focuses on enumeration and diagnostics deliverables.
- The minimal field set stays aligned with the canonical Week 2 deliverables: name, direction, channel count, default status, and sample-rate or format capability when available.
- Enumeration must flow through JUCE `AudioDeviceManager`.
- Enumeration output must be emitted outside the real-time audio path.
- Windows-first validation remains the target path; WSL is not accepted as real audio-device evidence.

## Daily Plan

### Day 1: Week 2 Scope And Enumeration Model

Date: 2026-06-30

Tasks:

- [x] Confirm the Week 2 scope against `docs/Plan/phase0-foundation-plan.md`.
- [x] Restate the Week 2 deliverables for device enumeration.
- [x] Define the minimal enumeration model and non-goals for this week.
- [x] Record that this week does not include real routing, virtual drivers, ASIO, or AirPlay.

Acceptance criteria:

- The Week 2 scope is explicit and matches the canonical Phase 0 plan.
- The minimal device field set is defined.
- Non-goals prevent expansion into routing or driver work.

### Day 2: `AudioDeviceInfo` And `DeviceEnumerator` Planning

Date: 2026-07-01

Tasks:

- [x] Plan the `engine/devices/AudioDeviceInfo` data structure.
- [x] Plan the initial `engine/devices/DeviceEnumerator` abstraction.
- [x] Define which fields are required now and which are diagnostics extensions.
- [x] Reserve room for future stable device IDs without expanding into Profile schema decisions that belong to Week 3.

Acceptance criteria:

- `AudioDeviceInfo` responsibilities are clearly described.
- `DeviceEnumerator` responsibilities are clearly described.
- Required and future fields are separated.

### Day 3: JUCE Integration And Output Visibility

Date: 2026-07-02

Tasks:

- [x] Plan how JUCE `AudioDeviceManager` is used for enumeration.
- [x] Decide whether the first visible output is shown in a minimal UI surface, logs, or both.
- [x] Ensure enumeration work is described as control-path work rather than callback work.
- [x] Document the distinction between input, output, duplex, and unknown device types.

Acceptance criteria:

- JUCE `AudioDeviceManager` is identified as the Week 2 integration point.
- The first visibility path for device lists is explicit.
- Enumeration is explicitly kept out of real-time callbacks.

### Day 4: Diagnostics Samples And Evidence Storage

Date: 2026-07-03

Tasks:

- [x] Plan the first example diagnostics output.
- [x] Define the minimum evidence fields to save: OS, audio interface, default input, and default output.
- [x] Plan the evidence drop location under `docs/evidence/`.
- [x] Record that platform-specific fields may be placed in an extension area while the minimal cross-platform field set stays stable.

Acceptance criteria:

- An example diagnostics-output direction is described.
- Evidence storage location is explicit.
- The evidence checklist matches the canonical Week 2 manual evidence requirements.

### Day 5: Week 2 Verification And Closeout

Date: 2026-07-04

Tasks:

- [x] Review the Week 2 acceptance criteria.
- [x] Confirm the week closes on enumeration visibility, input/output distinction, no Python dependency, and no real-time callback misuse.
- [x] Record which verification items require a real Windows environment.
- [x] Prepare the Week 3 entry point: Profile schema and diagnostics output compatibility.

Acceptance criteria:

- Week 2 remains strictly about enumeration and diagnostics.
- Verification boundaries are explicit.
- The Week 3 handoff is clear and does not claim real routing.

## Not-Run Items

- Real Windows device enumeration acceptance from the current WSL environment.
- Real routing or audio forwarding checks.
- Virtual driver integration checks.
- ASIO checks.
- AirPlay or network output checks.
- Profile schema validation, which belongs to Week 3.
