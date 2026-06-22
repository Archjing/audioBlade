# audioBlade

[![English](https://img.shields.io/badge/lang-English-blue.svg)](./README.md)
[![简体中文](https://img.shields.io/badge/lang-简体中文-red.svg)](./README.zh-CN.md)

audioBlade 是一个规划中的 Windows + macOS 桌面音频路由应用，目标是在不依赖 Studio One、Audition 等 DAW 软件的情况下，实现持久化系统音频路由、跳线和虚拟音频接口支持。

本仓库当前处于 Phase 0：基础工程阶段。

## 当前阶段

Phase 0 建立：

- 面向原生 C++/JUCE 构建的 CMake 所有权。
- 最小桌面应用目标名称。
- 基础架构目录。
- ADR 和开发者文档。
- 后续设备诊断与 profile schema 工作的落点。

Phase 0 Day 1 不实现真实音频路由、虚拟音频驱动、ASIO、AirPlay/网络输出、效果器、按应用路由，或后台服务拆分。

## JUCE 源码策略

JUCE 应作为固定版本源码 checkout 或 Git submodule 放在：

```text
extern/JUCE
```

当前 CMake 骨架会在 `extern/JUCE` 不存在时配置并构建一个占位 `audioBladeDesktop` 目标。该占位目标只用于证明构建系统结构，不会启动 GUI，也不验证音频行为。

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

## WSL 限制

WSL/Linux 检查可以验证仓库结构、CMake 配置和占位构建行为。它们不能证明：

- Windows WASAPI 行为。
- macOS Core Audio 行为。
- 物理音频设备可见性。
- 虚拟端点支持。
- GUI 启动质量。
- 实时回调安全性。
- 音频路由正确性。

这些检查后续必须基于宿主操作系统证据记录为 `not-run`、`pass` 或 `fail`。

## 项目文档

[![Agents](https://img.shields.io/badge/docs-AGENTS-2f6fbb.svg)](./AGENTS.md)
[![技术方案](https://img.shields.io/badge/docs-%E6%8A%80%E6%9C%AF%E6%96%B9%E6%A1%88-red.svg)](./docs/cn/技术方案.md)
[![Technical Plan](https://img.shields.io/badge/docs-Technical%20Plan-2f6fbb.svg)](./docs/TECHNICAL_PLAN.md)
[![开发规范](https://img.shields.io/badge/docs-%E5%BC%80%E5%8F%91%E8%A7%84%E8%8C%83-red.svg)](./docs/cn/开发规范.md)
[![开发流程](https://img.shields.io/badge/docs-%E5%BC%80%E5%8F%91%E6%B5%81%E7%A8%8B-red.svg)](./docs/DEVELOPMENT_PROCESS.md)
[![计划](https://img.shields.io/badge/docs-%E8%AE%A1%E5%88%92-red.svg)](./docs/Plan/)
[![Harness](https://img.shields.io/badge/docs-Harness-2f6fbb.svg)](./docs/Harness/)
[![ADR](https://img.shields.io/badge/docs-ADR-2f6fbb.svg)](./docs/adr/)
