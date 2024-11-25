# ACG-Project

## Description

- This is repo for course project of Advanced Computer Graphics on 2024 Fall, instructed by Li Yi (弋力).
- Team member: [Chenglin Liu](https://github.com/jk21lcl) (刘承林) and [He Li](https://github.com/lihe50hz) (李赫).
- The repo is based on https://github.com/Yao-class-graphics-studio/Sparkium-v2.

## Preparing work (For Visual Studio 2022 on Windows)

1. Download VS2022 from https://visualstudio.microsoft.com/.

2. Download CMake (at least version 3.22) from https://cmake.org/download/.

3. Download Vulkan from https://cpp.studio/toolchain/vulkan/.

4. Download CUDA from https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=WSL-Ubuntu&target_version=2.0&target_type=deb_local. *(ONLY FOR NVIDIA GPU)*

4. Clone this repo by `git clone --recurse-submodules https://github.com/lihe50hz/ACG-Project.git` to a folder that has a short prefix. 

   > Namely, your working directory path shouldn't be too long, since some code in this repo has a limitation of length 250 on the file path. 
   >
   > You'd better ensure the length of prefix is less than 150.

5. Set the `CMake Toolchain File` (namely `vcpkg.cmake`) in the `Project` -> `CMake settings`. (You may need specify this for your computer)

6. Add the `"builtin-baseline": "7adc2e4d49e8d0efc07a369079faa6bc3dbb90f3"` in the `vcpkg.json` file in the root directory. (Already done)

7. Try running the program.

## Project Details

- Base: 

  1. Implement a path tracing algorithm that correctly handles diffuse and specular materials. (basic)

     > He Li has implemented this.

- Scene creation:  

  1. Build a custom scene with aesthetic considerations, using geometry that you create from scratch or find online (ensure the source is credited). (basic, tidiness and attractiveness 1pt)

     > Waiting for register.

- Acceleration structure: 

  1. Implement an acceleration structure such as BVH (Bounding Volume Hierarchy). This is not required for hardware-based renderers, as the acceleration structure is built-in in that case. (basic, Surface Area Heuristic or another advanced algorithm 2pts)

     > This is not required for hardware.

- Material: Create a (non-trivial) custom material. Options include:

  1. Transmissive material (basic)

     > He Li has implemented this.

  2. Principled BSDF (2pts)

     > He Li has implemented this.

  3. Multi-layer material (2pts)

     > Waiting for register.

  4. Rendering of fur, hair, skin, etc. (2pts)

     > Waiting for register.

- Texture: Create your own (non-trivial) texture with proper texture mapping. Options include:

  1. Color texture (basic)

     > He Li has implemented this.

  2. Normal map, height map, attribute map, or any functional texture mapping (1pt for each, up to 2pts)

     > He Li has implemented normal map.

  3. Implement an adaptive mipmap algorithm (2pts)

     > Waiting for register.

- Importance Sampling: 

  1. Use more advanced sampling algorithms for path tracing. (Importance sampling with Russian Roulette, multiple importance sampling 2pts)

     > He Li has implemented this.

- Volumetric Rendering: Options include:

  1. Subsurface scattering (2pts)

     > Waiting for register.

  2. Homogeneous volume rendering (1pt)

     > Chenglin Liu has implemented this.

  3. Channel-independent subsurface scattering (1pt)

     > registered by Chenglin Liu.

  4. Inhomogeneous volume rendering (1pt)

     > registered by Chenglin Liu.

  5. Volumetric emission (1pt)

     > Chenglin Liu has implemented this.

  6. Volumetric alpha shadow (2pts)

     > registered by Chenglin Liu.

- Special Visual Effects: Options include:

  1. Motion blur, depth of field (basic)

     > Chenglin Liu has implemented this.

  2. Alpha shadow (basic)

     > Waiting for register.

  3. Cartoon style rendering (2pts)

     > Waiting for register.

  4. Chromatic dispersion (2pts)

     > Waiting for register.

- Lighting: Options include:

  1. Point light and area light (basic)

     > He Li has implemented this.

  2. Environment lighting with HDR, such as skybox (2pts)

     > He Li has implemented this.

- Anti-aliasing: Implement an anti-aliasing algorithm (basic)

  > He Li has implemented this.

- Simulation-based content creation: Up to 2pts

  > Waiting for register.

## Comment

1. From He Li's roommate: The material part contains many dirty work (Principled BSDF), but this is necessary for an attractive scene. Custom material is just about parameter fine-tuning.

# Sparkium Renderer (星火渲染器-开源版)

<!-- TOC -->
- [ACG-Project](#acg-project)
  - [Description](#description)
  - [Preparing work (For Visual Studio 2022 on Windows)](#preparing-work-for-visual-studio-2022-on-windows)
  - [Project Details](#project-details)
  - [Comment](#comment)
- [Sparkium Renderer (星火渲染器-开源版)](#sparkium-renderer-星火渲染器-开源版)
  - [简介](#简介)
  - [硬件要求](#硬件要求)
  - [编译环境配置](#编译环境配置)
    - [依赖项](#依赖项)
    - [编译步骤](#编译步骤)
    - [注意事项](#注意事项)
  - [如何上手](#如何上手)

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
