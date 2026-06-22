# audioBlade Harness Agents

## Purpose

This file is a lightweight routing index for audioBlade's Harness Engineering workflow. It should not duplicate the full product plan, architecture rules, or runner implementation.

Canonical sources:

- Product and engineering rules: `AGENTS.md`
- Harness workflow and artifact rules: `docs/Harness/workflow.md`
- Active Harness team charter: `docs/Harness/team.md`
- Technical rationale: `docs/TECHNICAL_PLAN.md`
- Development rules: `docs/DEVELOPMENT_GUIDELINES.md`
- Canonical Phase 0 plan: `docs/Plan/phase0-foundation-plan.md`
- Chinese translation of Phase 0 plan: `docs/cn/Plan/phase0-foundation-plan.cn.md`
- Long-term plan: `docs/cn/长期开发计划.md`
- Executable local profile: `/home/zj/workspace/codex-harness-runner/profiles/audioBlade.toml`
- Current runner role builders: `/home/zj/workspace/codex-harness-runner/codex_harness/agents.py`

## Context Hygiene

- Read this file only when the task is harness-style or role-based.
- For compare, admission, document revision, experiment report, or plan review tasks, read `docs/Harness/workflow.md` first.
- For team coordination, role conflict, or escalation questions, read `docs/Harness/team.md`.
- Do not copy this file's role notes into prompts together with full `AGENTS.md` and full `docs/` unless needed.
- Treat this file as an index; follow the canonical docs for actual product, architecture, driver, and verification rules.
- Inspect `/home/zj/workspace/codex-harness-runner` only when changing or validating the harness runner itself.
- For normal project work, reading `AGENTS.md` is enough unless the user explicitly invokes a harness role.

## Role Invocation Map

The audioBlade harness profile enables these project-specific specialists:

```toml
project_agents = [
  "audioblade_product_architect",
  "audioblade_realtime_driver_specialist",
  "audioblade_verification_engineer"
]
```

Use this map when the user asks for role-based work:

- `planner`: create a stage-aware, verifiable plan. Use `docs/Plan/` for canonical English plans and `docs/cn/Plan/` for Chinese translations.
- `audioblade_product_architect`: evaluate product boundary, MVP scope, route graph model, and roadmap fit.
- `audioblade_realtime_driver_specialist`: evaluate real-time audio, C++/JUCE, virtual endpoint, platform audio API, and driver risks.
- `audioblade_verification_engineer`: turn claims into reproducible checks and define validation evidence.
- `verifier`: run or assess configured verification commands.

Team tool names used by the local harness:

- `consult_audioblade_product_architect`
- `consult_audioblade_realtime_driver_specialist`
- `consult_audioblade_verification_engineer`

## Planner Contract

When the user says "让 planner 写一个计划" or similar, produce a plan that includes:

- start and end dates when scheduling is requested
- week-level milestones
- deliverables
- acceptance criteria
- verification commands or evidence
- risks and non-goals

For the first development round, keep the plan focused on the first verifiable foundation:

- CMake + JUCE project skeleton
- minimal desktop app entry
- device enumeration
- route/profile schema draft
- diagnostics output that can be checked before real routing exists

Do not start with AirPlay, first-party drivers, full VoiceMeeter replacement, effects, or per-application routing unless the user explicitly changes the strategy.
