# audioBlade Development Process Architecture

## Purpose

audioBlade is a polyglot desktop audio project. The development process must support C++/JUCE, platform audio APIs, future Rust control services, Python diagnostics/tooling, and Harness Engineering without allowing one toolchain to become an accidental owner of the whole project.

This document defines how the project should be maintained as it moves from documentation-first planning into implementation.

## Short Answer On uv

Using `uv` is a good idea for audioBlade's Python tooling layer, but it should not be the top-level build system for the whole product.

Use `uv` for:

- Python diagnostic scripts.
- Profile/schema validation tools.
- Documentation utilities.
- Harness helper scripts that live inside this repository.
- Reproducible Python environments via `pyproject.toml`, `.python-version`, and `uv.lock`.

Do not use `uv` to manage:

- C++/JUCE builds.
- Rust crates.
- Native driver SDKs.
- Windows driver signing state.
- macOS notarization state.
- External system services or hardware/audio-device setup.

The rule is simple:

> `uv` manages Python dependencies and Python command execution. CMake owns C++/JUCE. Cargo owns Rust. Platform-specific packaging and driver work stay explicit.

## Toolchain Ownership

### C++ / JUCE

Owner:

- `CMakeLists.txt`
- `CMakePresets.json`
- `apps/`
- `engine/`
- `platform/`
- `extern/` or another explicit dependency location for JUCE

Purpose:

- Desktop app.
- Real-time audio engine.
- Device enumeration.
- Audio callback and routing code.
- Platform audio API integration.

Policy:

- C++/JUCE is the real-time audio path.
- CMake is the source of truth for native builds.
- Use `CMakePresets.json` for shared configure/build presets.
- Use `CMakeUserPresets.json` only for local developer overrides and keep it out of Git.
- Pin JUCE explicitly. Prefer a Git submodule or pinned source checkout over an implicit floating dependency.

### Rust

Owner:

- `control/`
- `Cargo.toml`
- `Cargo.lock`
- `rust-toolchain.toml`

Purpose:

- Future control service.
- Profile/state management.
- IPC policy.
- Diagnostics orchestration.
- Update/lifecycle control.

Policy:

- Do not introduce Rust into the real-time audio buffer path.
- Add a Cargo workspace only when the Rust control service actually starts.
- Commit `Cargo.lock` for application/service reproducibility.
- Use `cargo test`, `cargo fmt`, and `cargo clippy` once Rust code exists.

### Python / uv

Owner:

- `pyproject.toml`
- `.python-version`
- `uv.lock`
- `tools/`

Purpose:

- JSON/Profile schema validation.
- Diagnostics post-processing.
- Documentation checks.
- Lightweight developer command wrappers.
- Test harness glue that is not part of the shipped real-time product.

Policy:

- Use `uv sync` to create the Python environment.
- Use `uv run ...` for Python tools so commands run with locked dependencies.
- Commit `uv.lock` when Python tooling becomes part of the repo.
- Keep Python outside real-time audio callbacks and OS virtual audio device exposure.
- Treat `uv sync` as dependency installation only. It does not provision external services, drivers, audio devices, databases, or platform SDKs.

### Harness Engineering

Owner:

- `docs/Harness/`
- `/home/zj/workspace/codex-harness-runner/profiles/audioBlade.toml`
- `/home/zj/workspace/codex-harness-runner/codex_harness/agents.py`

Purpose:

- Role-based planning and review.
- Context curation.
- Implementation orchestration.
- Verification planning.
- Durable documentation updates.

Policy:

- Harness instructions in this repository should be lightweight indexes, not duplicated product specs.
- Canonical product and technical rules stay in `AGENTS.md`, `docs/TECHNICAL_PLAN.md`, and `docs/DEVELOPMENT_GUIDELINES.md`.
- Canonical plans stay in English under `docs/Plan/`.
- Chinese readability copies stay under `docs/cn/`.
- Run the harness with the runner virtual environment:

```bash
cd /home/zj/workspace/codex-harness-runner
/home/zj/workspace/codex-harness-runner/.venv/bin/python3 main.py --profile audioBlade --mode plan "..."
```

## Recommended Repository Layout

The repository should evolve toward this structure:

```text
audioBlade/
  AGENTS.md
  CMakeLists.txt
  CMakePresets.json
  pyproject.toml
  uv.lock
  .python-version
  rust-toolchain.toml

  apps/
    desktop/

  engine/
    audio_graph/
    devices/
    realtime/
    routing/

  control/
    profiles/
      profile.schema.json
      examples/
    service/
    ipc/
    diagnostics/

  platform/
    windows/
    macos/

  extern/
    JUCE/

  tools/
    diagnostics/
    dev/

  docs/
    README.md
    TECHNICAL_PLAN.md
    DEVELOPMENT_GUIDELINES.md
    DEVELOPMENT_PROCESS.md
    Plan/
    Harness/
    adr/
    evidence/
    cn/
```

Not every directory needs to exist immediately. Create directories when there is a concrete file to put inside them.

## Command Architecture

Each layer keeps its native command surface:

```bash
# Native C++/JUCE
cmake --preset windows-dev
cmake --build --preset windows-dev
ctest --preset windows-dev

# Python tooling
uv sync
uv run python tools/diagnostics/validate_profile.py control/profiles/examples/default-monitor-route.json

# Rust control service, once added
cargo fmt
cargo clippy --all-targets
cargo test

# Harness runner, outside this repo
/home/zj/workspace/codex-harness-runner/.venv/bin/python3 main.py --profile audioBlade --mode plan "..."
```

If a single developer-facing command layer is needed later, add it as a thin wrapper that delegates to these native tools. Do not hide CMake, Cargo, or uv behind an opaque custom system.

Acceptable wrapper options:

- `tools/dev/dev.py` run with `uv run`.
- `justfile` if the team wants shell-style task aliases.
- CI workflow commands that call the native tools directly.

The wrapper may improve ergonomics, but native commands remain the source of truth.

## Phase-Based Process

### Phase 0

Goal:

- Buildable C++/JUCE skeleton.
- Device enumeration.
- Diagnostics output.
- Profile schema draft.

Process:

- CMake first.
- Python only for schema/diagnostic checks.
- No Rust service yet.
- No virtual driver integration.

### Phase 1

Goal:

- Local stereo route prototype.

Process:

- Extend C++/JUCE engine.
- Add audio-thread safety checks.
- Keep diagnostics outside callbacks.
- Begin real audio evidence capture.

### Phase 2

Goal:

- Virtual endpoint MVP.

Process:

- Integrate selected Windows virtual driver dependency.
- Document license and packaging risk.
- Keep driver dependency separate from route engine logic.

### Later Phases

Goal:

- Rust control service, production diagnostics, first-party driver decisions, macOS path, and network sinks.

Process:

- Introduce Cargo workspace only when control/service code exists.
- Add platform-specific packaging only when product behavior is already validated.
- Treat first-party drivers as separate projects with their own decision gates.

## Dependency Policy

### Commit These

- `CMakePresets.json`
- `pyproject.toml`
- `uv.lock`
- `.python-version`
- `Cargo.lock` once Rust application/service code exists
- `rust-toolchain.toml` once Rust code exists
- ADRs documenting dependency decisions

### Do Not Commit These

- `.venv/`
- `build/`
- `CMakeUserPresets.json`
- local IDE state
- local driver signing artifacts
- notarization credentials
- secrets or API keys

## Verification Matrix

Every substantial change should identify which layer it touches:

- `docs`: markdown/reference checks.
- `python-tools`: `uv run` checks.
- `cpp-build`: CMake configure/build.
- `cpp-audio`: device enumeration or audio routing evidence.
- `rust-control`: Cargo checks.
- `harness`: profile load, smoke test, or role invocation.
- `platform`: Windows/macOS-specific install, permission, driver, or packaging checks.

For Phase 0, the expected verification set is:

```bash
cmake -S . -B build
cmake --build build
uv sync
uv run python -m json.tool control/profiles/examples/default-monitor-route.json
```

Audio device verification must include human-readable evidence:

- OS version.
- Device names.
- Input/output direction.
- Channel count.
- Sample rate or format data when available.
- Command or UI path used to collect it.

## Decision Rules

- If the question is about real-time audio, C++/JUCE and CMake own the answer.
- If the question is about Python scripts or validation helpers, uv owns the answer.
- If the question is about Rust services, Cargo owns the answer.
- If the question is about project planning or role-driven work, Harness owns the workflow but not the product truth.
- If the question is about drivers, signing, notarization, or virtual audio devices, platform-specific documentation and ADRs must be written before implementation.

## Why This Split Works

This split avoids a common failure mode in mixed-stack projects: choosing one convenient tool and letting it become an accidental meta-build system.

For audioBlade:

- CMake is necessary because the product is a native desktop audio application.
- JUCE is appropriate for cross-platform audio app infrastructure.
- Rust is useful later for control-plane reliability, not immediate audio callbacks.
- Python is useful for tooling, diagnostics, and developer workflow.
- uv is useful because it makes Python tooling reproducible and fast.
- Harness Engineering is useful for planning and review, but should not duplicate canonical technical docs.

## References

- uv project guide: https://docs.astral.sh/uv/guides/projects/
- uv workspaces: https://docs.astral.sh/uv/concepts/projects/workspaces/
- CMake presets: https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html
- Rust Cargo workspaces: https://doc.rust-lang.org/book/ch14-03-cargo-workspaces.html
- JUCE CMake API: https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md

