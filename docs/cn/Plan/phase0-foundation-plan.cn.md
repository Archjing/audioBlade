# 第一基础模块开发计划

> 中文可读版。英文正本见 `docs/Plan/phase0-foundation-plan.md`。

## 计划定位

本计划由 planner 角色约束生成，用于 audioBlade 第一轮 Harness 开发。

目标不是直接做完整音频路由器，而是完成第一个最重要、可验证的基础功能模块：

> 一个可构建的 C++/JUCE 桌面应用骨架，能够枚举系统音频设备，输出可复现诊断信息，并建立后续路由与 Profile 持久化所需的基础模型。

该模块是后续 Windows 路由引擎、虚拟音频端点、Profile 恢复和后台生命周期的前置基础。

## 起止时间

- 开始日期：2026-06-23
- 结束日期：2026-07-20
- 周期：4 周
- 节奏：每周形成一个可检查增量，周末前完成验证记录

## 范围

### 包含

- CMake + JUCE 项目骨架
- 最小桌面应用入口
- 基础目录结构
- Windows 优先的音频设备枚举
- 设备诊断输出
- Route/Profile schema 初稿
- 最小自动化验证命令
- ADR 记录第一轮关键技术决策

### 不包含

- 虚拟音频驱动接入
- 自研驱动
- AirPlay 或网络输出
- 真实音频转发
- 后台服务拆分
- ASIO 支持
- 效果处理
- 完整路由矩阵 UI
- per-application routing

## 目标产物

第一基础模块完成时，仓库应具备：

- `CMakeLists.txt`
- `apps/desktop/` 最小 JUCE 应用
- `engine/devices/` 设备枚举抽象
- `control/profiles/` Profile schema 初稿
- `tools/diagnostics/` 设备诊断输出工具或脚本
- `docs/adr/` 至少一份 ADR
- 可运行的构建/诊断验证命令

## 成功标准

完成后必须能够证明：

- 项目可以在 Windows 开发环境中配置和构建
- 桌面应用可以启动到最小窗口或最小状态页
- 应用或诊断工具可以列出音频输入/输出设备
- 枚举结果至少包含设备名、输入/输出方向、通道数、采样率或可用格式信息
- 诊断输出可保存为文本或 JSON，便于后续 bug 复现
- Profile schema 能表达未来一路立体声 route 的 source、destination、gain、mute 和 sample-rate policy
- 所有验证步骤有命令或人工检查记录

## 周计划

### 第 1 周：项目骨架与构建基线

时间：2026-06-23 至 2026-06-29

目标：把文档型仓库变成可构建工程仓库。

交付物：

- 根目录 `CMakeLists.txt`
- `apps/desktop/` 最小 JUCE 应用结构
- `engine/`、`control/`、`platform/`、`tools/` 基础目录
- `docs/adr/0001-mvp-stack-and-boundaries.md`
- README 或开发说明中的最小构建命令

任务：

- 确定 JUCE 引入方式：Git submodule、FetchContent、系统路径或包管理器
- 建立 CMake target 命名约定
- 创建最小桌面应用入口
- 加入基础版本信息和应用名
- 写 ADR：第一阶段采用 C++/JUCE 单进程，Rust 控制服务暂缓

验收标准：

- CMake configure 可以通过
- 工程可以生成构建系统
- 最小应用 target 存在
- ADR 说明为什么第一阶段不拆 Rust 服务、不接虚拟驱动

验证命令或证据：

```bash
cmake -S . -B build
cmake --build build
test -f docs/adr/0001-mvp-stack-and-boundaries.md
```

风险：

- JUCE 获取方式在 Windows/WSL 混合环境中不稳定
- 当前 Linux 工作区不是实际 Windows 音频开发环境
- 过早设计复杂目录导致第一周没有可构建产物

缓解：

- 第一周只要求构建骨架，不要求真实音频功能
- Windows 专用验证可以用文档记录环境要求，避免在 WSL 中假装完成音频验证

### 第 2 周：设备枚举能力

时间：2026-06-30 至 2026-07-06

目标：实现第一个可见的音频能力：枚举系统输入/输出设备。

交付物：

- `engine/devices/AudioDeviceInfo` 数据结构
- `engine/devices/DeviceEnumerator` 初版
- JUCE AudioDeviceManager 集成
- UI 或日志中显示输入/输出设备列表
- 设备枚举诊断输出样例

任务：

- 定义设备信息字段：名称、方向、通道数、默认状态、采样率/格式能力
- 使用 JUCE 枚举当前系统音频设备
- 将枚举结果从实时音频路径外输出
- 区分 input、output、duplex 或未知类型
- 记录 Windows 优先验证方式

验收标准：

- 应用启动后可以列出可见音频设备
- 至少能区分输入设备和输出设备
- 枚举逻辑不依赖 Python
- 没有将枚举逻辑写入实时音频 callback

验证命令或证据：

```bash
cmake --build build
./build/apps/desktop/audioBlade --diagnose-devices
```

人工证据：

- 保存一次设备枚举输出到 `docs/evidence/`
- 记录 OS、音频接口、默认输入、默认输出

风险：

- JUCE 不同平台返回字段不一致
- Windows 音频设备名称和 ID 可能在重启后变化
- 当前开发环境可能无法访问真实 Windows 音频设备

缓解：

- 先定义跨平台最小字段，再把平台特有字段放入 diagnostics 扩展区
- WSL 环境只作为构建检查，不作为音频设备验收环境

### 第 3 周：Profile schema 与诊断输出

时间：2026-07-07 至 2026-07-13

目标：建立后续持久化路由的配置基础。

交付物：

- `control/profiles/profile.schema.json`
- `control/profiles/examples/default-monitor-route.json`
- `tools/diagnostics/` 诊断输出入口
- `docs/adr/0002-profile-schema-v0.md`
- Profile schema 校验说明

任务：

- 定义 `AudioNode`、`Port`、`Route`、`Profile` 的最小 JSON 表达
- 支持一路 stereo route 的 source/destination/gain/mute/sampleRatePolicy
- 为未来稳定设备 ID 与人类可读 fallback 预留字段
- 让设备枚举诊断输出可以关联 Profile schema 中的 device reference
- 记录哪些字段是 v0 必需字段，哪些是未来字段

验收标准：

- schema 可以表达一路虚拟输入到物理输出的目标形态
- schema 可以表达一路物理输入到虚拟输出的目标形态
- 示例 Profile 可通过 schema 校验
- 文档明确 v0 不执行实际路由，只定义可序列化模型

验证命令或证据：

```bash
test -f control/profiles/profile.schema.json
test -f control/profiles/examples/default-monitor-route.json
python3 -m json.tool control/profiles/examples/default-monitor-route.json >/dev/null
```

如引入 schema 校验工具，再增加：

```bash
python3 tools/diagnostics/validate_profile.py control/profiles/examples/default-monitor-route.json
```

风险：

- schema 设计过早复杂化
- Profile 和真实设备枚举 ID 绑定过紧，导致重连后不可恢复
- 把未来多通道、网络 sink、效果器提前塞入 v0

缓解：

- v0 只覆盖 stereo route 和必要诊断字段
- 多通道、网络、效果器只保留扩展能力，不进入第一模块验收

### 第 4 周：集成、验证与下一阶段入口

时间：2026-07-14 至 2026-07-20

目标：把骨架、设备枚举、诊断输出和 Profile schema 集成为可交付基础模块。

交付物：

- `docs/cn/第一基础模块验证报告.md`
- `docs/evidence/` 至少一份设备枚举样例
- 构建与诊断命令清单
- 下一阶段开发建议
- 已知风险清单

任务：

- 清理构建说明
- 补齐诊断输出格式说明
- 在 Windows 环境执行一次真实设备枚举
- 在 WSL/Linux 环境只执行构建或静态检查，不声称完成音频设备验收
- 对照 `AGENTS.md`、`docs/Harness/agents.md`、`docs/cn/长期开发计划.md` 检查范围是否漂移
- 确认下一阶段是否进入“一路输入到一路输出的本地 stereo route”

验收标准：

- 新开发者可以按文档配置、构建、运行诊断
- 验证报告包含实际命令、输出摘要和未验证项
- 第一模块没有引入虚拟驱动、自研驱动、AirPlay 或真实路由承诺
- 下一阶段入口明确

验证命令或证据：

```bash
cmake -S . -B build
cmake --build build
./build/apps/desktop/audioBlade --diagnose-devices
python3 -m json.tool control/profiles/examples/default-monitor-route.json >/dev/null
rg -n "AirPlay|first-party|VoiceMeeter|ASIO" docs/cn/第一基础模块验证报告.md
```

风险：

- 真实 Windows 环境验证没有及时完成
- 设备枚举输出格式频繁变化
- 第一模块被扩展成真实路由开发，导致节奏失控

缓解：

- 验证报告允许标记 `not_run`，但必须说明原因
- 输出格式进入 v0 后只做兼容性扩展
- 把真实音频转发留给第二基础模块

## 任务分解

### Planner

- 保持计划节奏和范围
- 每周复核是否仍然服务于第一个可验证基础模块
- 防止范围提前扩展到虚拟驱动或网络输出

### Implementer

- 建立 CMake/JUCE 工程
- 实现设备枚举和诊断输出
- 建立 Profile schema 与示例

### Reviewer

- 审查实时音频边界是否被破坏
- 审查是否过早引入虚拟驱动、ASIO、AirPlay 或效果器
- 审查文档和实现是否一致

### Verifier

- 执行构建检查
- 执行设备枚举检查
- 校验 JSON 示例
- 编写验证报告

## 最小完成定义

第一基础模块只有在以下条件都满足时才算完成：

- 仓库可以构建最小桌面应用
- 可以运行设备枚举诊断
- 有一次真实或明确标记限制的设备枚举证据
- Profile schema 和示例存在且可校验
- ADR 记录了第一阶段关键技术选择
- 验证报告列出通过项、失败项和未运行项

## 下一阶段入口

完成本计划后，下一阶段应进入：

> Windows 本地 stereo route 原型：选择一个输入设备和一个输出设备，连续转发立体声音频，并记录延迟、CPU 和掉音情况。

仍然不进入：

- 虚拟驱动接入
- 第一方驱动研发
- AirPlay/network sink
- 完整路由矩阵 UI
