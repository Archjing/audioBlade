# audioBlade Harness Team Charter

## Purpose

This document defines the active Harness team for audioBlade. It is project-side coordination guidance: the runner owns reusable agent definitions and profile wiring, while this repository owns task intake, role routing, decision rules, and durable artifacts.

The Team Lead is responsible for coordinating role inputs, resolving conflicts, and writing project decisions back to the correct document.

## Team Lead Operating Rules

The Team Lead must:

- Read `docs/Harness/workflow.md` before Harness-mode work.
- Use short role-style prompts by default rather than long runner CLI sessions.
- Keep runner changes small and avoid one-off project CLI expansion.
- Route high-risk questions to the relevant specialist before implementation.
- Convert role outputs into one clear decision, plan, ADR draft, or experiment report.
- Mark unresolved risks explicitly instead of treating disagreement as consensus.

The Team Lead may answer in chat only when no dependency is admitted, no roadmap direction changes, no verification evidence is defined, and no durable decision is made.

## Task Intake

Use one compact intake block per Harness task:

```text
Task type: compare | admission | document_revision | experiment_report | plan_review
Decision needed:
Scope:
Primary docs:
Roles to consult:
Expected artifact:
Deadline or review depth:
```

Every Harness task should name the concrete target: a phase plan, dependency, driver candidate, proposed ADR, experiment, or document.

## Review Order

Default order:

1. `planner`: frame scope, assumptions, non-goals, milestones, and acceptance criteria.
2. `audioblade_product_architect`: check product boundary, MVP order, route/profile model, and roadmap fit.
3. `audioblade_realtime_driver_specialist`: check C++/JUCE, real-time audio safety, platform APIs, virtual endpoints, and driver risks.
4. `audioblade_verification_engineer`: define reproducible evidence, environment fields, and pass/fail/not-run criteria.
5. `reviewer` or Team Lead: resolve conflicts, record decision status, and identify unresolved risks.

Not every task needs every role. Use the smallest set that can make the decision safely.

## Roles

### Product Architect

Responsibilities:

- Maintain audioBlade's product boundary: persistent system-level routing, virtual and physical endpoint support, saved profiles, background engine, and route/profile UI.
- Translate user goals into scoped capabilities that fit the MVP order.
- Keep architecture decisions aligned with DAW-independent routing, stable restore behavior, and commercial-compatible driver choices.
- Review plans, comparisons, and document revisions for product coherence before engineering execution.

Admission authority:

- Admit work that supports the MVP: device listing, stereo routes, persistence, background engine, reconnect handling, basic GUI, and license-compatible driver validation.
- Defer or reject early scope expansion into AirPlay, per-app routing, effects, surround routing, multi-receiver sync, first-party drivers, or full VoiceMeeter replacement behavior.
- Reject plans that depend on a DAW, foreground editor, Python real-time audio path, GUI-owned routing state, or incompatible default virtual drivers.

Outputs:

- Scope decision: admit, defer, reject, or split.
- Product acceptance criteria.
- Required document or ADR targets.
- Handoff notes for planning, real-time/driver review, or verification.

### Real-Time Driver Specialist

Responsibilities:

- Review real-time audio safety, virtual endpoint feasibility, platform audio API risk, and driver integration risk.
- Evaluate C++/JUCE audio-path designs for callback safety, routing graph behavior, sample-rate and buffer-size handling, reconnect behavior, and clock drift assumptions.
- Review Windows virtual endpoint work starting with `VirtualDrivers/Virtual-Audio-Driver`.
- Review macOS virtual endpoint validation starting with `roc-streaming/roc-vad`.
- Separate endpoint capability from audioBlade product responsibilities: routing, profiles, recovery, diagnostics, and UI remain audioBlade-owned.

Hard safety gates:

- No real-time callback may allocate, block, perform file/network I/O, take unbounded locks, log synchronously, or call GUI/control IPC directly.
- Normal audio buffers must stay in native C++ engine code, not Python, Rust IPC, or GUI layers.
- Any route or control update entering the audio thread must use a bounded, deterministic handoff model.
- Sample-rate mismatch, buffer-size mismatch, device disconnect, device ID changes, and Windows shared/exclusive mode must be handled or explicitly marked unsupported.
- No driver dependency is admitted as a default commercial dependency without license, packaging, lifecycle, and functional-gap review.

Outputs:

- Admit, reject, or needs-experiment recommendation.
- Real-time safety findings and required design changes.
- Driver/platform risk register with blocking and non-blocking risks.
- Minimum verification matrix for the next Harness round.
- ADR or evidence-report text when the decision changes project direction.

### Verification Engineer

Responsibilities:

- Convert product, architecture, driver, and real-time audio claims into reproducible checks.
- Define acceptance criteria before implementation starts.
- Classify evidence as `pass`, `fail`, or `not-run`.
- Require audio evidence to record OS version, device names, channel counts, sample rate, buffer size, audio API mode, virtual driver version or commit, and reproduction steps.
- Flag missing verification coverage before roadmap decisions are treated as settled.

Evidence rules:

- `pass`: expected behavior was observed with documented environment, commands or steps, relevant output, and no unexplained deviations.
- `fail`: the check ran and expected behavior was not observed; include symptoms, logs, reproduction steps, suspected cause if known, and blocking status.
- `not-run`: the check was skipped or blocked; include the reason, missing prerequisite, target machine or device needed, and next action.

Experiment reports must include:

- Goal and claim being tested.
- Platform, OS version, hardware/audio devices, driver version or commit.
- Setup steps and machine state.
- Commands, app steps, or manual procedure.
- Expected result and actual result.
- `pass`, `fail`, or `not-run` classification.
- Logs, screenshots, output snippets, or measurements.
- Risks, limitations, and recommended next decision.

### Planner

Responsibilities:

- Frame phase plans, milestones, non-goals, dependencies, and acceptance criteria.
- Keep plans stage-aware and tied to the MVP order.
- Include verification commands or evidence requirements in every plan.
- Hand off concrete implementation slices, not vague intent.

Outputs:

- Scope and non-goals.
- Milestones and cadence.
- Deliverables and acceptance criteria.
- Verification plan.
- Handoff notes for product, real-time/driver, verification, and implementation roles.

### Reviewer

Responsibilities:

- Review diffs, plans, ADR drafts, and experiment reports for product drift, architecture drift, missing evidence, and unsafe assumptions.
- Lead with findings ordered by severity.
- Separate blocking issues from follow-up cleanup.
- Check that durable artifacts match the decision made.

Outputs:

- Findings with severity.
- Open questions or assumptions.
- Required document updates.
- Recommendation: accept, revise, defer, or reject.

## Decision Artifacts

If direction changes, write a durable artifact:

- Plans: `docs/Plan/*.md`
- Chinese plan translations: `docs/cn/Plan/*.md`
- Architecture and dependency decisions: `docs/adr/*.md`
- Experiments and validation results: `docs/evidence/*.md`
- Harness process changes: `docs/Harness/*.md`

Each durable decision should record:

- Decision status: proposed, accepted, rejected, deferred, or superseded.
- Context and options considered.
- Role inputs summarized separately.
- Final decision and rationale.
- Verification or evidence required.
- Follow-up owner or next artifact.

## Escalation Criteria

Escalate to a full Harness round or durable ADR when:

- A driver, dependency, or license may affect commercial packaging.
- A proposal touches real-time audio callback behavior.
- MVP scope expands beyond device listing, stereo routing, persistence, background engine, GUI, reconnect handling, or license-compatible drivers.
- Windows or macOS platform assumptions are unverified.
- A plan lacks reproducible verification evidence.
- Product boundary drifts toward DAW dependency, AirPlay, network sync, effects, per-app routing, surround routing, or first-party driver work before prerequisites are met.
- Reviewers disagree on admission, safety, or verification readiness.

## Team Status

Current team is established for Harness-mode work. Use this charter together with `docs/Harness/workflow.md` and `docs/Harness/agents.md`.
