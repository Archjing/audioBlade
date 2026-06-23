# Evidence

This directory stores reproducible verification evidence for Phase 0 and later phases.

Phase 0 Week 2 expects at least one device-enumeration sample to be saved here when a real Windows audio environment is available.

Minimum evidence fields for a Week 2 device-enumeration sample:

- date
- host OS and version
- machine or environment label
- audio API/backend when known
- default input device
- default output device
- visible device list
- command used to produce the output
- result classification: `pass`, `fail`, or `not_run`

WSL-generated static or build evidence may also be recorded here, but it must not be mislabeled as real Windows audio-device acceptance.
