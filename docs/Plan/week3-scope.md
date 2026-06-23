# Phase 0 Week 3 Scope

## Purpose

Week 3 establishes the configuration foundation for later persistent routing. It defines a serializable Profile schema, one example Profile, diagnostics compatibility expectations, and an ADR for the v0 model boundary.

## Deliverables

- `control/profiles/profile.schema.json`
- `control/profiles/examples/default-monitor-route.json`
- `tools/diagnostics/validate_profile.py`
- `docs/adr/0002-profile-schema-v0.md`
- Week 3 verification notes in `docs/Plan/week3-daily-plan.md`

## V0 Boundary

The v0 model is limited to one stereo route. It can describe a future route from a virtual input to a physical output, or from a physical input to a virtual output, but it does not execute either route.

Required route concepts:

- source node and source ports
- destination node and destination ports
- gain
- mute
- `sampleRatePolicy`
- stable identifier fields when available
- human-readable fallback fields when identifiers change

## Non-Goals

Week 3 does not include:

- real routing or audio forwarding
- virtual endpoint implementation
- driver integration
- background engine lifecycle
- profile restore
- multichannel route execution
- network output, AirPlay, or effects processing

## Harness Role Notes

Planner: keep Week 3 scoped to schema, example, diagnostics compatibility, and verification.

Product Architect: ensure the v0 model supports the MVP order without admitting VoiceMeeter-scale scope.

Real-Time Driver Specialist: confirm no real-time callback or driver behavior is introduced by schema work.

Verification Engineer: classify Week 3 checks as schema and JSON validation only, not audio behavior validation.
