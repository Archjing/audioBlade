# Phase 0 Week 3 Daily Plan

## Purpose

This is the daily execution plan for Phase 0 Week 3. It expands the Week 3 section of `docs/Plan/phase0-foundation-plan.md`.

Week 3 goal:

> Establish the configuration foundation for later persistent routing by defining the initial Profile schema and diagnostics output compatibility.

## Week 3 Dates

- Start: 2026-07-07
- End: 2026-07-13

## Decisions For Week 3

- Week 3 defines a serializable model and diagnostics compatibility; it does not implement real routing.
- The v0 schema stays limited to one stereo route with source, destination, gain, mute, and `sampleRatePolicy`.
- The schema must reserve room for stable device IDs and human-readable fallback matching.
- Diagnostics compatibility is required so future device references can align with enumeration output.
- Multichannel, network sink, effects, and virtual-driver-specific semantics remain future extensions.

## Daily Plan

### Day 1: Week 3 Scope And Schema Boundaries

Date: 2026-07-07

Tasks:

- [x] Confirm the Week 3 scope against `docs/Plan/phase0-foundation-plan.md`.
- [x] Restate the Week 3 deliverables: schema, example profile, diagnostics entry point, and ADR.
- [x] Fix the v0 boundary around stereo routes and serialization only.
- [x] Record explicit non-goals for real routing, virtual endpoints, and multichannel expansion.

Acceptance criteria:

- The Week 3 scope is explicit and matches the canonical Phase 0 plan.
- The v0 schema boundary is clear.
- Non-goals prevent expansion beyond serialization and diagnostics compatibility.

### Day 2: Profile Model Planning

Date: 2026-07-08

Tasks:

- [x] Define the minimal JSON representations for `AudioNode`, `Port`, `Route`, and `Profile`.
- [x] Plan how one stereo route is represented.
- [x] Plan fields for source, destination, gain, mute, and `sampleRatePolicy`.
- [x] Reserve fields for stable device IDs and human-readable fallback matching.

Acceptance criteria:

- The core v0 model is described in terms of canonical Week 3 entities.
- One stereo route can be represented in the planned model.
- Future fallback matching is reserved without over-expanding the schema.

### Day 3: Diagnostics Compatibility And Evidence Planning

Date: 2026-07-09

Tasks:

- [ ] Align Week 3 schema planning with Week 2 device enumeration output.
- [ ] Describe how diagnostics fields can reference future Profile device entries.
- [ ] Plan the evidence items needed for `docs/Plan/phase0-verification-report.md`.
- [ ] Ensure diagnostics compatibility is recorded without claiming real routing behavior.

Acceptance criteria:

- Diagnostics compatibility expectations are explicit.
- Evidence needs for the later verification report are listed.
- No real routing behavior is implied.

### Day 4: Verification Commands And Not-Run Boundaries

Date: 2026-07-10

Tasks:

- [ ] List the canonical Week 3 verification commands.
- [ ] Record the optional schema validation path if a validation tool is introduced later.
- [ ] Define output summary expectations and `not_run` classification rules for schema-related checks.
- [ ] Confirm that schema validation is distinct from route execution.

Acceptance criteria:

- Verification commands align with the canonical Week 3 plan.
- Optional validation-tool handling is explicit.
- Not-run items are separated from pass/fail claims.

### Day 5: Week 3 Closeout And Phase 0 Alignment

Date: 2026-07-11

Tasks:

- [ ] Review the Week 3 acceptance criteria.
- [ ] Confirm the schema can represent future virtual-input-to-physical-output and physical-input-to-virtual-output routes as serializable models only.
- [ ] Record the Phase 0 risk list for schema creep.
- [ ] Prepare the Week 4 entry point: integration, verification, and next-phase recommendation.

Acceptance criteria:

- Week 3 stays within schema and diagnostics-output planning.
- Future route examples are described as model capability, not implemented behavior.
- The Week 4 handoff is clear and within Phase 0 scope.

## Not-Run Items

- Real route execution or audio forwarding.
- Virtual input or virtual output implementation.
- Multichannel route validation.
- Network sink or AirPlay-related schema expansion.
- Effects-processing schema expansion.
