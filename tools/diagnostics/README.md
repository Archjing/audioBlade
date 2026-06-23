# Diagnostics Tools

This directory contains developer diagnostics helpers for Phase 0 and later phases.

Python tools here are allowed because they validate files and evidence outside the real-time audio path. They must not process live audio buffers or expose OS-level virtual audio devices.

## Profile Validation

Run:

```bash
python3 tools/diagnostics/validate_profile.py control/profiles/examples/default-monitor-route.json
```

The validator performs JSON syntax checks and basic Profile v0 consistency checks. If the optional `jsonschema` Python package is installed, it also validates against `control/profiles/profile.schema.json`.
