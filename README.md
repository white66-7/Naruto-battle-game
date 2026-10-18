<div align="center">

# 火影格斗 FolkFight

**基于 SFML 3 与 Box2D 构建的 2D 横版格斗小游戏**

[![C++](https://img.shields.io/badge/C++-20-00599C.svg?style=flat-square&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-3.0.2-8CC445.svg?style=flat-square&logo=sfml&logoColor=white)](https://www.sfml-dev.org/)
[![Box2D](https://img.shields.io/badge/Box2D-3.1.1-B9292E.svg?style=flat-square)](https://box2d.org/)
[![MinGW](https://img.shields.io/badge/MinGW--w64-GCC%2016.2-F0A030.svg?style=flat-square&logo=gcc&logoColor=white)](https://www.mingw-w64.org/)
[![Windows](https://img.shields.io/badge/Windows-10%2B-0078D4.svg?style=flat-square&logo=windows&logoColor=white)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](LICENSE)

<p align="center">
  <a href="#项目概述">项目概述</a> •
  <a href="#直接运行">直接运行</a> •
  <a href="#功能特性">功能特性</a> •
  <a href="#技术栈">技术栈</a> •
  <a href="#项目结构">项目结构</a> •
  <a href="#从源码编译">从源码编译</a>
</p>

---

</div>

## 项目概述

本项目是大一上学期（2025.09 – 2025.12）用 C++ 手写的 2D 横版格斗游戏原型，基于 **SFML** 完成图形渲染与音频播放，基于 **Box2D** 完成物理模拟与碰撞检测。

最初在 Visual Studio 2022 中开发，2026 年 7 月迁移至 VSCode，并重新整理为 **单文件全静态发布** 的形式——下载即玩，不需要安装任何运行库。

<img width="1192" height="1122" alt="游戏截图" src="https://github.com/user-attachments/assets/aa57895e-bc34-46df-b27a-3eb2d233e993" />

---

## 直接运行

**下载仓库后开箱即用，无需安装任何东西。**

```text
output/
├── FolkFight.exe   ← 双击运行
└── resources/      ← 游戏素材，必须与 exe 同级
```

> `FolkFight.exe` 为**全静态链接**的 Release 版本，SFML、Box2D、FreeType、
> HarfBuzz、OpenAL、FLAC、Vorbis 等依赖已全部编译进可执行文件内部，
> **不依赖任何第三方 DLL，也无需安装 VC++ 运行库**。
> 仅需 64 位 Windows 10 及以上系统。

---

## 功能特性

### 角色与动画
- **多帧动画系统**：支持待机、奔跑、起跳、攻击、遁地、发波等动画状态切换与帧计时。
- **主角动作**：基于 `sasuke` 类实现输入响应、动画驱动与物理体同步。

### 物理与战斗
- **Box2D 3.1 物理世界**：重力模拟、静态地形碰撞体、平台落点判定。
- **碰撞检测**：基于 AABB 的精确碰撞测试，用于攻击判定。
- **镜头跟随**：摄像机根据角色位置平滑更新视口。

### 场景与资源
- **纹理与精灵**：统一的纹理加载流程，逐帧裁剪渲染。
- **音频播放**：菜单音乐、攻击音效与技能音效三通道支持。
- **UI 与字体**：基于 SFML 文本渲染的菜单界面。

---

## 技术栈

| 分类 | 核心技术 / 依赖 | 说明 |
| :--- | :--- | :--- |
| **语言标准** | C++20 | 使用 `std::filesystem`、结构化绑定等特性 |
| **图形与窗口** | SFML 3.0.2 | 渲染、事件、窗口管理 |
| **音频** | SFML Audio + OpenAL | 音效与背景音乐播放 |
| **物理引擎** | Box2D 3.1.1 | 刚体、形状与碰撞响应 |
| **字体渲染** | FreeType + HarfBuzz | SFML 文本依赖链 |
| **构建工具链** | MinGW-w64 (GCC 16.2, UCRT64) | 全静态链接构建 |
| **编辑器** | Visual Studio Code | C/C++ 扩展 + 自定义构建任务 |

---

## 项目结构

```text
battle/
├── output/
│   ├── FolkFight.exe           # 全静态可执行文件（发布产物）
│   └── resources/              # 游戏素材（PNG / OGG / MP3 / TTF）
├── main.cpp                    # 主程序：游戏循环、场景构建与 UI
├── susake.cpp                  # 角色模块实现
├── build_release.bat           # 一键全静态构建脚本
├── LICENSE                     # MIT 开源协议
├── README.md
└── .vscode/                    # 编辑器配置
    ├── tasks.json              # 构建任务
    ├── launch.json             # 运行 / 调试配置
    └── c_cpp_properties.json   # IntelliSense 头文件路径
```

---

## 从源码编译

### 环境准备

- **MinGW-w64**（推荐 MSYS2 的 UCRT64 变体，GCC 13+）
- 静态库：SFML 3.0.x、Box2D 3.1.1 及其依赖（FreeType、HarfBuzz、OpenAL、FLAC、Vorbis、libpng、zlib）

### 构建步骤

1. 将静态库放入 `.dl/mingwlibs/ucrt64/`（`include/` 与 `lib/` 目录）
2. 修改 [build_release.bat](build_release.bat) 顶部的 `MINGW` 变量，指向你的 `mingw64/bin`
3. 执行构建：

```bash
# Windows 命令行
build_release.bat
```

产物输出至 `output/FolkFight.exe`，同样是全静态单文件，不依赖任何外部 DLL。

### 在 VSCode 中

```text
Ctrl + Shift + B   # 执行构建任务（build-release）
F5                 # 选择 "Run FolkFight (built exe)" 运行
```

`launch.json` 提供两个配置：

| 配置 | 用途 |
| :--- | :--- |
| `Run FolkFight (built exe)` | 构建后直接以集成终端运行，日常使用选这个 |
| `Attach with gdb (...)` | 需要断点调试时使用，要求 `toolchain/gdb/gdb.exe` 可正常工作 |

两个配置的 `preLaunchTask` 均为 `build-release`，按 F5 时会自动先编译再启动。

---

## 已知限制

- **断点调试暂不可用**：当前 MinGW 工具链附带的 `gdb.exe` 依赖了与
  MSYS2 现有 `gettext` 版本不兼容的导入符号（`libintl-8.dll` 缺少 `rpl_getcwd`），
  启动时会报 `ENTRYPOINT_NOT_FOUND`。日常运行不受影响，代码层面的错误由
  VSCode C/C++ 扩展的 IntelliSense 实时提示。如需断点调试，可另行安装
  匹配版本的 `gdb`，或改用 Visual Studio 的 MSVC 工具链重新编译 Debug 版。
- **游戏为半成品**：角色招式、多关卡与结算逻辑尚未完成。

---

## 开源协议与版权声明

本项目基于 **MIT 许可协议** 开源，详见 [LICENSE](LICENSE)。

- **技术代码**：可自由用于学习、修改与分发。
- **美术与音频素材**：`resources/` 目录内的图片、音频素材版权归原作者所有，
  仅供学习交流，**严禁商用**。
