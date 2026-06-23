# Week 3 Verification

## Purpose

This document records the canonical Week 3 verification commands and classifies what they prove.

## Commands

Required file checks:

```bash
test -f control/profiles/profile.schema.json
test -f control/profiles/examples/default-monitor-route.json
test -f docs/adr/0002-profile-schema-v0.md
```

JSON syntax checks:

```bash
python3 -m json.tool control/profiles/profile.schema.json >/dev/null
python3 -m json.tool control/profiles/examples/default-monitor-route.json >/dev/null
```

Profile validation helper:

```bash
python3 tools/diagnostics/validate_profile.py control/profiles/examples/default-monitor-route.json
```

## Expected Output Summary

The validation helper should print:

```text
profile validation passed: control/profiles/examples/default-monitor-route.json (...)
```

The validation mode may be:

- `jsonschema+basic` when the optional `jsonschema` package is installed.
- `basic` when only the repository-local consistency checks run.

## Pass Criteria

Week 3 schema validation is `pass` when:

- the schema file exists
- the example profile exists
- both files parse as JSON
- the validation helper exits with code `0`
- the example profile references known node and port IDs

## Not-Run Classification

Mark checks as `not_run` when:

- optional `jsonschema` package validation is unavailable
- Windows or macOS audio devices are not available
- route execution is requested
- virtual endpoint installation is requested

## Boundary

These commands validate configuration files only. They do not prove route execution, audio forwarding, driver behavior, profile restore, or real-time callback safety.
