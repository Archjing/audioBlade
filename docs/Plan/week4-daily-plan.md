# Phase 0 Week 4 Daily Plan

## Purpose

This is the daily execution plan for Phase 0 Week 4. It expands the Week 4 section of `docs/Plan/phase0-foundation-plan.md`.

Week 4 goal:

> Integrate the skeleton, device enumeration, diagnostics output, and Profile schema into a deliverable foundation module with verification evidence and a clear next-phase entry.

## Week 4 Dates

- Start: 2026-07-14
- End: 2026-07-20

## Decisions For Week 4

- Week 4 is an integration and verification week, not a feature-expansion week.
- The verification report is the main closeout artifact.
- Real Windows evidence is preferred, but `not_run` classification is allowed when the required environment is unavailable.
- WSL/Linux checks remain build or static checks only.
- Phase 0 must not claim virtual drivers, first-party drivers, AirPlay, or real routing.

## Daily Plan

### Day 1: Integration Scope And Closeout Boundaries

Date: 2026-07-14

Tasks:

- Confirm the Week 4 scope against `docs/Plan/phase0-foundation-plan.md`.
- Restate the required closeout artifacts: verification report, evidence, command list, next-phase recommendation, and known risks.
- Fix the boundary that Week 4 integrates prior work instead of adding new product scope.
- Reconfirm excluded claims: virtual drivers, first-party drivers, AirPlay, and real routing.

Acceptance criteria:

- The Week 4 scope is explicit and matches the canonical Phase 0 plan.
- Closeout artifacts are listed.
- Out-of-scope claims are explicitly blocked.

### Day 2: Verification Report Evidence Structure

Date: 2026-07-15

Tasks:

- Plan the structure of `docs/Plan/phase0-verification-report.md`.
- Gather the evidence categories that must be present: command evidence, output summary, and `not_run` items.
- Align Week 4 closeout with the minimum definition of done from the canonical plan.
- Confirm that diagnostics evidence and schema evidence are both represented.

Acceptance criteria:

- The verification report structure is defined.
- Evidence categories match the canonical Week 4 and minimum-definition-of-done requirements.
- Diagnostics and schema evidence are both covered.

### Day 3: Verification Commands And Output Summary

Date: 2026-07-16

Tasks:

- Consolidate the canonical build and diagnostics commands for Phase 0 closeout.
- Define how output summaries should be recorded.
- Record which checks can run in WSL/Linux and which require a real Windows environment.
- Ensure `not_run` handling is part of the report plan rather than omitted.

Acceptance criteria:

- The closeout command list is explicit.
- Output summary expectations are explicit.
- Environment-specific verification boundaries are documented.

### Day 4: Risks, Known Limitations, And Next Phase Entry

Date: 2026-07-17

Tasks:

- Consolidate the known risks list from Weeks 1 to 4.
- Record the allowed limitation language for missing real Windows evidence.
- Confirm the next phase recommendation: a Windows local stereo route prototype.
- Ensure the next phase entry is framed as the next step rather than Phase 0 completion scope.

Acceptance criteria:

- Risks and limitations are explicit.
- The next phase recommendation matches the canonical plan.
- Phase 0 closeout language does not overclaim.

### Day 5: Phase 0 Final Scope Check

Date: 2026-07-18

Tasks:

- Perform the final scope check against `AGENTS.md`, `docs/Harness/agents.md`, and `docs/cn/长期开发计划.md` as required by the canonical Week 4 plan.
- Confirm the repository docs allow a new developer to configure, build, and run diagnostics from documentation.
- Confirm the final closeout does not state or imply virtual drivers, real routing, or AirPlay support.
- Record any remaining blockers as `fail` or `not_run`.

Acceptance criteria:

- Scope consistency is checked against the listed documents.
- Documentation handoff expectations are explicit.
- Remaining blockers are classified rather than hidden.

## Not-Run Items

- Any real Windows verification not available during Week 4 execution.
- Virtual driver integration checks.
- First-party driver development checks.
- AirPlay or network sink checks.
- Real local stereo routing checks, which belong to the next phase.
