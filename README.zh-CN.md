# audioBlade

[![English](https://img.shields.io/badge/lang-English-blue.svg)](./README.md)
[![简体中文](https://img.shields.io/badge/lang-简体中文-red.svg)](./README.zh-CN.md)

audioBlade 是一个规划中的 Windows + macOS 桌面音频路由应用，目标是在不依赖 Studio One、Audition 等 DAW 软件的情况下，实现持久化系统音频路由、跳线和虚拟音频接口支持。

本仓库当前处于 Phase 0：基础工程阶段。Week 3 已完成，项目正在进入 Week 4 验证收口。

## 当前阶段

Phase 0 已建立：

- 面向原生 C++/JUCE 构建的 CMake 所有权。
- 最小桌面应用目标。
- 基础架构目录。
- 基于 JUCE 的 Windows-first 设备枚举。
- 包含 backend、direction、channel count 和 OS metadata 的设备诊断输出。
- 面向未来持久化 stereo route 的 Profile schema v0。
- ADR、Harness 工作流文档、证据和阶段收口报告。

当前状态：

- Week 1：项目骨架和构建基线已完成。
- Week 2：Windows 设备枚举已完成，并已记录证据。
- Week 3：Profile schema 和 diagnostics compatibility 已完成。
- Week 4：下一步是验证报告、集成摘要、风险列表和下一阶段建议。

Phase 0 仍不实现真实音频路由、虚拟音频驱动、ASIO、AirPlay/网络输出、效果器、按应用路由、profile restore，或后台服务拆分。

## JUCE 源码策略

JUCE 应作为固定版本源码 checkout 或 Git submodule 放在：

```text
extern/JUCE
```

当前桌面目标是 `audioBladeDesktop`。当 `extern/JUCE` 存在时，它会构建 JUCE 桌面应用，并支持 Phase 0 diagnostics 参数。

## 配置与构建

在仓库根目录执行：

```bash
cmake -S . -B build
cmake --build build
```

使用 preset：

```bash
cmake --preset dev
cmake --build --preset dev
```

## 设备诊断

在 Windows 宿主机构建后运行：

```powershell
.\build\apps\desktop\audioBladeDesktop_artefacts\Debug\audioBlade.exe --diagnose-devices
```

Week 2 证据记录在：

```text
docs/evidence/week2-device-enumeration-sample.md
```

## Profile 验证

Profile schema v0 和示例文件位于：

```text
control/profiles/
```

验证示例 profile：

```bash
python3 -m json.tool control/profiles/profile.schema.json >/dev/null
python3 -m json.tool control/profiles/examples/default-monitor-route.json >/dev/null
python3 tools/diagnostics/validate_profile.py control/profiles/examples/default-monitor-route.json
```

## WSL 限制

WSL/Linux 检查可以验证仓库结构、CMake 配置、JSON/profile 验证，以及部分构建行为。它们不能证明：

- Windows WASAPI 行为。
- macOS Core Audio 行为。
- 物理音频设备可见性。
- 虚拟端点支持。
- GUI 启动质量。
- 实时回调安全性。
- 音频路由正确性。

当前 WSL 构建备注：完整 JUCE audio-device 构建可能需要 Linux 音频开发头文件，例如 ALSA。Windows 宿主机构建和设备枚举证据仍是 Week 2 音频设备验证的权威路径。

## 项目文档

[![Agents](https://img.shields.io/badge/docs-AGENTS-2f6fbb.svg)](./AGENTS.md)
[![技术方案](https://img.shields.io/badge/docs-%E6%8A%80%E6%9C%AF%E6%96%B9%E6%A1%88-red.svg)](./docs/cn/技术方案.md)
[![Technical Plan](https://img.shields.io/badge/docs-Technical%20Plan-2f6fbb.svg)](./docs/TECHNICAL_PLAN.md)
[![开发规范](https://img.shields.io/badge/docs-%E5%BC%80%E5%8F%91%E8%A7%84%E8%8C%83-red.svg)](./docs/cn/开发规范.md)
[![开发流程](https://img.shields.io/badge/docs-%E5%BC%80%E5%8F%91%E6%B5%81%E7%A8%8B-red.svg)](./docs/DEVELOPMENT_PROCESS.md)
[![计划](https://img.shields.io/badge/docs-%E8%AE%A1%E5%88%92-red.svg)](./docs/Plan/)
[![Harness](https://img.shields.io/badge/docs-Harness-2f6fbb.svg)](./docs/Harness/)
[![ADR](https://img.shields.io/badge/docs-ADR-2f6fbb.svg)](./docs/adr/)
