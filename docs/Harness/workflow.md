# audioBlade Harness Workflow

## Purpose

audioBlade uses Harness Engineering as a review and coordination method, not as a replacement for the normal project build tools.

The runner repository owns reusable agent definitions and local execution plumbing. This repository owns the project process: what work should go through Harness mode, which documents are canonical, and what evidence must be written back after each round.

## Three-Layer Workflow

### 1. External Runner Setup

The external runner lives at:

```text
/home/zj/workspace/codex-harness-runner
```

Runner-side responsibilities:

- Maintain the `audioBlade` profile at `/home/zj/workspace/codex-harness-runner/profiles/audioBlade.toml`.
- Maintain reusable audioBlade project agents in `/home/zj/workspace/codex-harness-runner/codex_harness/agents.py`.
- Keep the runner close to the OpenAI Agents SDK pattern: define `Agent(...)` objects and expose them through existing runner orchestration.
- Keep runner changes small. Do not add project-specific runner CLI complexity unless there is a repeated operational need.

Project-side documents should not duplicate runner implementation details. They should only reference the runner profile and role names needed for audioBlade work.

### 2. Project Integration Documentation

The project-level Harness entry points are:

- `AGENTS.md`
- `docs/Harness/agents.md`
- `docs/Harness/workflow.md`
- `docs/Harness/team.md`
- `docs/README.md`

These files define when Harness mode should be used, which project documents are canonical, and what artifacts a Harness round must produce.

Use `docs/Harness/team.md` for active team responsibilities, review order, escalation criteria, and Team Lead operating rules.

Canonical project context remains in:

- `docs/TECHNICAL_PLAN.md`
- `docs/DEVELOPMENT_GUIDELINES.md`
- `docs/DEVELOPMENT_PROCESS.md`
- `docs/Plan/`
- `docs/adr/` once ADRs exist
- `docs/evidence/` once experimental evidence exists

Do not put every phase plan into the runner profile `rules`. Phase plans should be passed explicitly in the task prompt or discovered through `docs/README.md` and this workflow document.

### 3. Harness-Mode Development Tasks

Use Harness mode for work that benefits from multiple viewpoints before code changes or product decisions.

Preferred Harness-mode task types:

- `compare`: compare libraries, drivers, APIs, products, or architecture options.
- `admission`: decide whether a candidate dependency, driver, feature, or scope item may enter the roadmap.
- `document_revision`: revise plans, ADRs, technical notes, or process docs with reviewer-style checks.
- `experiment_report`: design or review reproducible experiments and write evidence reports.
- `plan_review`: review phase plans before implementation and check scope, risk, and verification.

Normal implementation tasks may still be done directly in the project repository. Harness mode should be used when there is meaningful architecture risk, driver risk, licensing risk, real-time audio risk, or planning ambiguity.

## Recommended Role Routing

Use these audioBlade project specialists when relevant:

- `audioblade_product_architect`: product boundary, MVP staging, route/profile model, roadmap fit.
- `audioblade_realtime_driver_specialist`: C++/JUCE, real-time audio safety, platform audio APIs, virtual endpoint and driver risks.
- `audioblade_verification_engineer`: reproducible verification, experiment design, evidence format, pass/fail/not-run classification.

Use built-in runner roles for:

- `planner`: phase plans, milestone breakdown, acceptance criteria.
- `reviewer`: findings-first review of diffs, plans, and risks.
- `verifier`: command/evidence-based verification.
- `memory_steward`: durable project documentation updates.

## Artifact Rules

Every Harness-mode round should leave a durable project artifact when it changes project direction.

Use these locations:

- Plans: `docs/Plan/*.md`
- Chinese plan translations: `docs/cn/Plan/*.md`
- ADRs: `docs/adr/*.md`
- Experiment reports: `docs/evidence/*.md`
- Harness process notes: `docs/Harness/*.md`

If a Harness round only answers a question and does not change direction, a chat answer is enough. If it admits a dependency, rejects a feature, changes scope, or defines verification evidence, write it into the appropriate project document.

## Prompt Pattern

Use direct role prompts rather than relying on the runner to guess the task.

Examples:

```text
Use Harness mode for compare:
Compare VirtualDrivers/Virtual-Audio-Driver and a SYSVAD-derived first-party path for Windows Phase 1 admission. Write the result as an ADR draft.
```

```text
Use Harness mode for plan_review:
Review docs/Plan/phase0-foundation-plan.md for scope drift, missing verification, and readiness for implementation.
```

```text
Use Harness mode for experiment_report:
Design a Windows device-enumeration experiment. Define commands, required machine state, evidence fields, pass/fail/not-run criteria, and report location.
```

## KISS Rule

Prefer the simplest working coordination path:

- Keep reusable roles and profile wiring in the external runner.
- Keep project workflow and artifact rules in this repository.
- Do not grow runner CLI entry points for one-off project tasks.
- Do not duplicate full project context into every prompt.
- Do not run a long Harness process when a short role emulation and durable document update is enough.
