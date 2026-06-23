# Week 3 Diagnostics Compatibility

## Purpose

This note aligns the Week 3 Profile schema with the Week 2 device-enumeration diagnostics output. It defines how future profile device references can be compared against diagnostic evidence without claiming that routes are executable in Phase 0.

## Source Diagnostics

Primary Week 2 evidence:

- `docs/evidence/week2-device-enumeration-sample.md`

The diagnostic output currently includes:

- `date`
- `os`
- `device_count`
- `backend`
- `name`
- `direction`
- `input_channels`
- `output_channels`
- `default_input`
- `default_output`
- optional `current_sample_rate`

## Profile Matching Fields

Profile v0 device references use `node.match` fields.

| Profile field | Diagnostics source | Notes |
| --- | --- | --- |
| `match.backendType` | `backend=` line | Required because the same device name may appear under multiple Windows audio modes. |
| `match.displayName` | `name=` field | Human-readable match target and UI label fallback. |
| `match.direction` | `direction=` field | Keeps input, output, duplex, and unknown devices separate. |
| `match.stableId` | future diagnostics field | Reserved for platform identifiers once exposed reliably. |
| `match.fallback.nameContains` | `name=` field | Optional fallback when stable IDs change. |
| `match.fallback.minimumInputChannels` | `input_channels=` field | Avoids matching an output-only device for an input node. |
| `match.fallback.minimumOutputChannels` | `output_channels=` field | Avoids matching an input-only device for an output node. |

## Duplicate Name Rule

Week 2 evidence shows repeated device names across these backend modes:

- `Windows Audio`
- `Windows Audio (Exclusive Mode)`
- `Windows Audio (Low Latency Mode)`
- `DirectSound`

This is expected Windows/JUCE behavior. Profile matching must not rely on `name` alone. Use at least:

```text
backendType + displayName + direction
```

Stable platform identifiers should become preferred when available.

## Evidence Needed For Phase 0 Verification Report

The final Phase 0 verification report should include:

- profile schema path
- example profile path
- JSON syntax validation command
- schema validation helper command if present
- source device enumeration evidence path
- statement that route execution was not tested
- statement that default-device mapping remains a follow-up if no default device is marked true

## Not-Run Boundaries

Diagnostics compatibility does not prove:

- route execution
- device reopen behavior
- profile restore after restart
- virtual-driver endpoint availability
- sample-rate conversion
- buffer-size negotiation
- reconnect recovery

Those checks belong to later route-engine and lifecycle phases.
