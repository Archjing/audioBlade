# Contributing to audioBlade / 参与 audioBlade 贡献

[English](#english) | [简体中文](#简体中文)

## English

audioBlade is an early-stage Windows + macOS desktop audio routing project. Contributions are welcome, especially from people interested in real-time audio, virtual audio devices, desktop UX, diagnostics, and cross-platform native development.

This project is documentation-first during Phase 0. Please read the project context before making substantial changes:

- [AGENTS.md](./AGENTS.md)
- [Technical Plan](./docs/TECHNICAL_PLAN.md)
- [Development Guidelines](./docs/DEVELOPMENT_GUIDELINES.md)
- [Development Process](./docs/DEVELOPMENT_PROCESS.md)
- [Harness Workflow](./docs/Harness/workflow.md)

## Contribution Workflow

Use pull requests for all non-trivial changes.

Before opening a PR:

- Keep the change focused.
- Explain the motivation and expected impact.
- Include verification steps, or explain why verification is not available.
- Update related docs when behavior, architecture, process, or user-facing expectations change.

For code changes, include the smallest useful build, test, or diagnostic evidence. WSL/Linux checks are useful for repository structure and CMake sanity, but they do not prove Windows WASAPI, macOS Core Audio, virtual driver, GUI, or real-time audio behavior.

## Documentation Change Policy

Most documentation is open to contributor changes through PR review.

Normal documentation changes:

- `README.md`
- `README.zh-CN.md`
- `docs/DEVELOPMENT_GUIDELINES.md`
- `docs/DEVELOPMENT_PROCESS.md`
- `docs/Plan/*.md`
- `docs/reports/*.md`
- `docs/logs/*.md`
- `docs/cn/*.md`

These should describe what changed and why. A maintainer review is expected.

Higher-impact documentation changes:

- `docs/TECHNICAL_PLAN.md`
- `docs/cn/技术方案.md`
- `docs/adr/*.md`
- `docs/Harness/*.md`
- `AGENTS.md`

These files affect architecture, workflow, agent behavior, or long-term project direction. PRs are welcome, but they must include rationale, affected areas, and any compatibility or process impact. Maintainer approval is required before merge.

## ADR Policy

Architecture Decision Records are append-first.

Do not silently rewrite accepted decisions. If a decision changes:

- Add a new ADR.
- Mark the older ADR as superseded when appropriate.
- Link the old and new ADRs.

Historical context matters for future contributors and agents.

## Agent and Harness Instructions

`AGENTS.md` and `docs/Harness/*.md` are operational instructions for Codex/Harness workflows. Contributor PRs may improve them, but they require maintainer review because they can change how automated or agent-assisted work is performed.

## License and Driver Dependencies

Virtual audio driver dependencies must be open source and compatible with future commercial use unless the project explicitly decides otherwise.

Before proposing a driver dependency, include:

- Repository URL.
- License URL.
- Commercial-use assessment.
- Functional gap assessment.
- Packaging and signing risks.

Repository documentation is engineering guidance, not legal advice. Commercial distribution requires formal license review.

## 简体中文

audioBlade 是一个早期阶段的 Windows + macOS 桌面音频路由项目。欢迎对实时音频、虚拟音频设备、桌面 UX、诊断工具和跨平台原生开发感兴趣的贡献者参与。

Phase 0 阶段本项目以文档优先。进行较大改动前，请先阅读项目上下文：

- [AGENTS.md](./AGENTS.md)
- [技术方案](./docs/cn/技术方案.md)
- [开发规范](./docs/cn/开发规范.md)
- [开发流程](./docs/DEVELOPMENT_PROCESS.md)
- [Harness 工作流](./docs/Harness/workflow.md)

## 贡献流程

所有非微小改动都应通过 Pull Request 提交。

提交 PR 前：

- 保持改动聚焦。
- 说明改动动机和预期影响。
- 提供验证步骤；如果无法验证，说明原因。
- 当行为、架构、流程或用户可见预期发生变化时，同步更新相关文档。

代码改动应提供最小但有用的构建、测试或诊断证据。WSL/Linux 检查可用于验证仓库结构和 CMake 基础状态，但不能证明 Windows WASAPI、macOS Core Audio、虚拟驱动、GUI 或实时音频行为。

## 文档修改策略

大多数文档都允许 contributor 通过 PR 修改。

普通文档改动：

- `README.md`
- `README.zh-CN.md`
- `docs/DEVELOPMENT_GUIDELINES.md`
- `docs/DEVELOPMENT_PROCESS.md`
- `docs/Plan/*.md`
- `docs/reports/*.md`
- `docs/logs/*.md`
- `docs/cn/*.md`

这些改动应说明改了什么以及为什么改。通常需要 maintainer review。

高影响文档改动：

- `docs/TECHNICAL_PLAN.md`
- `docs/cn/技术方案.md`
- `docs/adr/*.md`
- `docs/Harness/*.md`
- `AGENTS.md`

这些文件会影响架构、流程、agent 行为或长期项目方向。欢迎通过 PR 改进，但必须说明理由、影响范围，以及兼容性或流程影响。合并前需要 maintainer approval。

## ADR 策略

架构决策记录应优先采用追加式变更。

不要静默重写已经接受的决策。如果决策发生变化：

- 新增一条 ADR。
- 必要时将旧 ADR 标记为已被替代。
- 在新旧 ADR 之间互相链接。

历史上下文对未来 contributor 和 agent 都很重要。

## Agent 与 Harness Instructions

`AGENTS.md` 和 `docs/Harness/*.md` 是 Codex/Harness 工作流的操作 instructions。Contributor 可以通过 PR 改进这些内容，但它们可能改变自动化或 agent-assisted work 的执行方式，因此必须由 maintainer review。

## License 与驱动依赖

除非项目明确做出不同决策，虚拟音频驱动依赖必须开源，并兼容未来商业使用。

提出驱动依赖前，请提供：

- 仓库 URL。
- 许可证 URL。
- 商业使用评估。
- 功能差距评估。
- 打包、签名和分发风险。

仓库文档只是工程指导，不是法律意见。商业发行前需要正式许可证审查。
