# 🎮 火影格斗

> 大一上学期花了三个月（2025.09 – 2025.12）写的 C++ 小游戏，基于 **SFML** 和 **Box2D** 实现。<br>
>做了一半，后面摆烂了，一直手搓效率实在是太慢了<br>
> 2026年7月初从 Visual Studio 迁移到了 VSCode后提交到GitHub，代码虽然又长又臭，UI 也蛮丑陋的，但毕竟是新手嘛，其实现在也能理解

---
<img width="1192" height="1122" alt="屏幕截图 2026-07-04 212038" src="https://github.com/user-attachments/assets/aa57895e-bc34-46df-b27a-3eb2d233e993" />

## ✨ 功能特性

- 🎨 **2D 图形渲染**：基于 SFML 实现
- 🎵 **音频播放支持**：SFML Audio 模块
- ⚙️ **物理模拟**：集成 Box2D 物理引擎
- 🖥️ **Windows 平台支持**（x86 / x64）

---

## 🛠️ 技术栈

| 组件 | 说明 |
|------|------|
| **语言** | C++（ISO C++20 / 最新标准） |
| **编译器** | MSVC v143（Visual Studio 2022） |
| **图形 / 音频** | SFML（Simple and Fast Multimedia Library） |
| **物理引擎** | Box2D v3.1.1 |
| **构建系统** | MSBuild（Visual Studio 项目） |
| **目标平台** | Windows 10.0+（Win32 / x64） |
| **开发环境** | 原 Visual Studio 2022 → 现已迁移至 VSCode |

---

## 📦 依赖库安装

### 1. SFML
- 下载地址：https://www.sfml-dev.org/download.php
- 解压到 `C:\Users\12625\Desktop\SFML\`
- 目录结构应包含 `include/` 和 `lib/`

### 2. Box2D
- 仓库地址：https://github.com/box2d/box2d
- 版本要求：v3.1.1
- 解压到 `D:\box\box2d-3.1.1\`
- 需自行编译生成 `box2dd.lib`（Debug）和 `box2d.lib`（Release）

---

## 🔧 编译配置

### 支持配置

| 配置 | 平台 | 说明 |
|------|------|------|
| Debug | Win32 | 调试版，32位 |
| Release | Win32 | 发布版，32位 |
| Debug | x64 | 调试版，64位（推荐开发用） |
| Release | x64 | 发布版，64位（推荐最终发布） |

### Debug x64 配置详情

| 项目 | 内容 |
|------|------|
| **附加包含目录** | `C:\Users\12625\Desktop\SFML\include`<br>`D:\box\box2d-3.1.1\include` |
| **附加库目录** | `C:\Users\12625\Desktop\SFML\lib`<br>`D:\box\box2d-3.1.1\build\src\Debug` |
| **链接依赖** | `box2dd.lib`<br>`sfml-window-d.lib`<br>`sfml-system-d.lib`<br>`sfml-graphics-d.lib`<br>`sfml-audio-d.lib` |

### Release x64 配置详情

| 项目 | 内容 |
|------|------|
| **附加包含目录** | `C:\Users\12625\Desktop\SFML\include`<br>`D:\box\box2d-3.1.1\include` |
| **附加库目录** | `C:\Users\12625\Desktop\SFML\lib`<br>`D:\box\box2d-3.1.1\build\src\Release` |
| **链接依赖** | `box2d.lib`<br>`sfml-window.lib`<br>`sfml-system.lib`<br>`sfml-graphics.lib`<br>`sfml-audio.lib` |
## 📁 项目结构
```text
火影格斗/
├── 测试.vcxproj # Visual Studio 项目文件
├── 测试.vcxproj.filters # 文件筛选器（如有）
├── main.cpp # 主程序入口
├── susake.cpp # 核心功能实现（角色、战斗逻辑等）
└── README.md # 本文件
```
> 文件名“测试”是当初随便起的

## 🚀 编译与运行

### 使用 Visual Studio 2022

1. 双击 `.vcxproj` 文件打开项目
2. 选择目标配置（如 `Debug | x64`）
3. 按 `Ctrl + Shift + B` 编译
4. 按 `F5` 启动调试
