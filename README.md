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

     > He Li has implemented this.

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

## Acknowledgement

1. The project the based on https://github.com/Yao-class-graphics-studio/Sparkium-v2. This is a legal start point announced in the document.

2. The theory of the implementation is based on PBRT book https://www.pbr-book.org/. The code from this book is partly referred, but we are doing GPU rendering while the PBRT mainly focus on CPU rendering. 

3. The code for Principled BSDF is based on blog https://schuttejoe.github.io/post/disneybsdf/. 

   > However, we don't recommend anyone use this post as a start for other projects. Although our code seems similar to this, the details are different in many aspects. This tutorial contains many subtle mistakes, which cost me even more time debugging than creating from the scratch. 

4. Some code are generated by Generative AI like ChatGPT or Copilot, but most of them are modified to fit into the project.

## Reproduction

1. Follow our instruction to prepare your environment.
2. Hit the green triangle, you should see the rendering result, all the parameters can be tuned in GUI to create the demos, which is what we have done.
