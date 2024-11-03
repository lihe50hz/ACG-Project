# Sparkium Renderer (星火渲染器-开源版)

<!-- TOC -->
* [Sparkium Renderer (星火渲染器-开源版)](#sparkium-renderer-星火渲染器)
  * [简介](#简介)
  * [硬件要求](#硬件要求)
  * [编译环境配置](#编译环境配置)
    * [依赖项](#依赖项)
    * [编译步骤](#编译步骤)
    * [注意事项](#注意事项)
  * [如何上手](#如何上手)
<!-- TOC -->

## 简介

Sparkium Renderer 是一个基于 Vulkan API 的渲染器，旨在提供一个简单易用的渲染器框架，以便于学习和研究。
星火取自“星星之火，可以燎原”，希望这个渲染器能够为更多人点燃学习和研究渲染的火花，同时点点星火也隐含着光源在渲染中的重要性。

## 硬件要求

本渲染器基于 Vulkan Ray Tracing 扩展，因此需要支持 Vulkan Ray Tracing 的显卡才能运行。
一般来说，以下常见显卡型号支持 Vulkan Ray Tracing：

- NVIDIA GeForce RTX 20 系列及以上
- AMD Radeon RX 6000 系列及以上
- Intel Arc 系列

如果你的显卡不支持 Vulkan Ray Tracing，请不要尝试运行本渲染器。

**免责声明**： 本渲染器仅供学习和研究使用，不保证其在所有环境下都能正常运行。目前作者仅在 NVIDIA 显卡上测试过本渲染器，其他显卡可能存在兼容性问题。

## 编译环境配置

### 依赖项

本项目依赖以下外部内容：

- [CMake](https://cmake.org/)
- [Vulkan SDK](https://vulkan.lunarg.com/)
- [Python3](https://www.python.org/)
- [vcpkg](https://github.com/microsoft/vcpkg)

请提前安装或克隆好以上依赖项。

### 编译步骤

1. 克隆项目

```shell
git clone <link-to-this repository>
```

2. 进入项目目录

```shell
cd Sparkium
```

3. 创建 build 目录

```shell
mkdir build
cd build
```

4. 配置 CMake

```shell
cmake -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake ..
```

5. 编译项目

```shell
cmake --build .
```

6. 运行项目

```shell
./code/sparkium/sparkium
```

### 注意事项

vcpkg 安装的库可能会有不同版本，如果编译出现问题，请检查 vcpkg 安装的库版本是否正确。

vcpkg 在下载库时可能会出现下载失败的情况，可以尝试多次下载或者使用代理。


## 如何上手

请参照 [docs/getting_started.md](docs/getting_started.md) 查看上手指南。

此外，你还可以查看 [docs/definition.md](docs/definition.md) 了解更多关于本项目的定义和结构。以及 [docs/framework.md](docs/framework.md) 了解本项目的架构。
