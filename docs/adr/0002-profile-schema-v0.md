# ADR 0002: Profile Schema V0

## Status

Accepted for Phase 0.

## Date

2026-06-23

## Context

audioBlade needs persistent routing profiles, but Phase 0 does not execute audio routes. Week 3 establishes the serializable configuration model that later phases can use for route restore, diagnostics, and compatibility checks.

The schema must stay small enough to validate now while leaving room for stable device identifiers, human-readable fallback matching, and later virtual endpoint integration.

## Decision

Profile schema v0 defines a serializable model only.

It admits these model entities:

- `AudioNode`: a physical, virtual, or future application/network endpoint reference.
- `Port`: a channel-level reference on an `AudioNode`.
- `Route`: a stereo connection from source ports to destination ports.
- `Profile`: a named route set with route defaults and metadata.

The v0 schema supports one stereo route with:

- source device reference
- destination device reference
- left and right channel mapping
- gain
- mute state
- `sampleRatePolicy`

The v0 schema reserves room for:

- stable platform device identifiers when available
- backend or API mode names from diagnostics output
- human-readable fallback matching when stable IDs change

## Non-Goals

Profile schema v0 does not implement:

- real audio routing
- virtual audio endpoints
- driver installation or driver selection
- route execution
- multichannel routing
- effects processing
- AirPlay, network sinks, or multi-receiver sync
- per-application capture

## Consequences

Positive:

- Later routing work can target a concrete profile shape instead of ad hoc configuration.
- Week 2 diagnostics output can be aligned with future profile device references.
- Profile validation can run before any real-time audio path exists.

Tradeoffs:

- Device references may still need platform-specific improvement after real Windows and macOS driver work begins.
- Default device resolution is not considered complete until diagnostics can map concrete defaults reliably.
- v0 intentionally excludes multichannel and network concepts that may be needed later.

## Verification

Minimum Week 3 checks:

```bash
test -f control/profiles/profile.schema.json
test -f control/profiles/examples/default-monitor-route.json
python3 -m json.tool control/profiles/examples/default-monitor-route.json >/dev/null
```

If the validation helper is present:

```bash
python3 tools/diagnostics/validate_profile.py control/profiles/examples/default-monitor-route.json
```

These checks validate schema shape and JSON compatibility only. They do not prove audio routing, virtual-driver behavior, or profile restore behavior.
