# 上手指南

<!-- TOC -->
* [上手指南](#上手指南)
  * [编写着色器](#编写着色器)
  * [编辑场景](#编辑场景)
  * [修改着色器资源](#修改着色器资源)
  * [添加着色器资源](#添加着色器资源)
  * [调试工具](#调试工具)
<!-- TOC -->

## 编写着色器

着色器是渲染管线的核心，它决定了渲染的效果。你需要打开 [code/sparkium/renderer/shaders/raytracing.rgen](../code/sparkium/renderer/shaders/raytracing.rgen)
，找到函数 `SampleRay` 的实现，然后参考已经给出的示例代码，编写你自己的光线追踪算法。
需要注意的是，本项目会在编译阶段将着色器文件嵌入到生成的二进制文件中，所以你需要在修改着色器文件后重新编译项目才能看到效果。而由于目前仍然存在的
CMake 配置问题，修改 `*.glsl` 文件不会触发重新编译，所以当你仅对 `*.glsl` 文件进行修改时，需要手动修改一下
`raytracing.rgen` 文件，比如敲一个无意义的空格或回车，然后重新编译项目。

## 编辑场景

场景的定义都位于 [code/sparkium/app/built_in_scenes.h](../code/sparkium/app/built_in_scenes.h)
中，你可以参照已有的场景定义，添加你自己的场景定义函数。并将你的场景定义函数添加到 `BuiltInSceneList` 中，这样你就可以在程序中选择你的场景了。

## 修改着色器资源

着色器内已经绑定的资源类型已经提供了大量有用信息，足够满足绝大多数渲染算法。
但如果你需要添加新的元素，比如在 EntityMetadata 中添加一个新的字段，你需要懂得如何修改着色器资源。
目前框架里所有着色器资源涉及到的结构体都存在一个 C++ 部分的定义和一个 GLSL 部分的定义，这两部分的定义需要保持一致。

还是以 EntityMetadata 为例，目前 EntityMetadata
的定义分别位于 [code/sparkium/scene/entity.h](../code/sparkium/scene/entity.h)
和 [code/sparkium/renderer/shaders/entity_metadata.glsl](../code/sparkium/renderer/shaders/entity_metadata.glsl)
中，你需要同时修改这两个文件。
C++ 的定义如下：

```cpp
struct EntityMetadata {
  glm::mat4 transform{1.0f};
  uint32_t entity_id{0};
  uint32_t mesh_id{0};
  uint32_t albedo_texture_id{0};
  uint32_t albedo_detail_texture_id{0};
  glm::vec4 detail_scale_offset{10.0f, 10.0f, 0.0f, 0.0f};
  float emission_cdf{0.0f};
  float padding0;  // padding to 16 bytes
  float padding1;  // padding to 16 bytes
  float padding2;  // padding to 16 bytes
  // This structure needs to be padded to 16 bytes
  // If you wants to add normal_texture_id, you should add it here.
};
```

GLSL 的定义如下：

```glsl
struct EntityMetadata {
    mat4 model;
    uint entity_id;
    uint mesh_id;
    uint albedo_texture_id;
    uint albedo_detail_texture_id;
    vec4 detail_scale_offset;
    float emission_cdf;
    float padding0;
    float padding1;
    float padding2;
// align to 16 bytes
};
```

你需要保证这两个定义的字段顺序和类型完全一致，以及需要注意到，由于 GPU 的对齐要求，结构体的大小需要是 16
字节的倍数，所以你需要在结构体的最后添加一些 padding。
目前的 EntityMetadata 结构体已经添加了 3 个 padding 字段，当你添加新的字段时，需要根据需要修改 padding 字段的数量。（当结构体的内容自然对齐到
16 字节时，不需要添加 padding 字段）

**注意**，向量类型 `vec*` 也需要将起始地址对齐到 16 字节，具体对齐要求可以搜索 `std140` 和 `std430` 对齐规则以及 GLSL
的相关文档。如果发现对齐结果和预期不符，可以尝试调整字段的顺序或者添加 padding 字段。

加入你需要在 EntityMetadata 中添加一个新的字段 `normal_texture_id`，用于表示法线贴图的索引，你需要在 C++ 部分添加这个字段，并在
GLSL 部分添加这个字段，同时修改 padding 字段的数量。
参考结果如下：

```cpp
struct EntityMetadata {
  glm::mat4 transform{1.0f};
  uint32_t entity_id{0};
  uint32_t mesh_id{0};
  uint32_t albedo_texture_id{0};
  uint32_t albedo_detail_texture_id{0};
  glm::vec4 detail_scale_offset{10.0f, 10.0f, 0.0f, 0.0f};
  float emission_cdf{0.0f};
  uint32_t normal_texture_id{0};
  float padding0;  // padding to 16 bytes
  float padding1;  // padding to 16 bytes
  // This structure needs to be padded to 16 bytes
};
```

```glsl
struct EntityMetadata {
    mat4 model;
    uint entity_id;
    uint mesh_id;
    uint albedo_texture_id;
    uint albedo_detail_texture_id;
    vec4 detail_scale_offset;
    float emission_cdf;
    uint normal_texture_id;
    float padding0;
    float padding1;
// align to 16 bytes
};
```

## 添加着色器资源

着色器调用的资源是通过一系列 Vulkan API 操作绑定到着色器上的，主要流程大致如下：

1、创建一个描述符集合布局（Descriptor Set Layout，下称“布局”）。布局可以理解为一个参数列表，描述了着色器需要的资源类型和数量。每一个布局中的绑定项都对应了一个着色器中
`layout(set = X, binding = Y) uniform/buffer` 的声明。

2、创建一个描述符池（Descriptor
Pool）。描述符池是描述符集合的分配器，需要提前指定总的描述符数量和集合数量上限。当你添加新的布局绑定项时，需要对应增加描述符池的总描述符数量。如果你是在既有描述符布局的基础上进行添加的，那么框架的代码已经自动处理计算描述符池参数的逻辑，不需要手动修改。

3、创建一个描述符集合（Descriptor
Set）。描述符集合可以理解为函数调用时的实参列表，描述了着色器需要的具体资源。描述符集合需要从描述符池中分配（Allocate）。然后在每次渲染调用之前，将当次渲染需要的资源绑定到描述符集合上。

4、创建一个管线布局（Pipeline Layout）。管线布局是描述符集合布局的集合，描述了着色器需要的所有资源。因为不同的模块，如
Renderer，AssetManager，Scene都各自维护各自的布局以增加灵活性，所以需要将这些布局合并到一个管线布局中供着色器统一调用。

描述符相关的代码都写在 Renderer、AssetManager、Scene
等模块中，你可以在这些模块中找到对应的代码进行修改，具体修改方法建议参考已有相关内容的实现。注意，修改描述符布局时，需要同时修改对应的描述符池和描述符集合的创建逻辑，以及设计好对应的资源绑定逻辑。

如果对相关概念感到不易理解，可以参考 Vulkan API 的相关文档，或者在 Issues 中提问。

## 调试工具

推荐使用 [Nsight Graphics](https://developer.nvidia.com/nsight-graphics) 进行调试，Nsight Graphics 是 NVIDIA 提供的一款强大的 GPU 调试工具，可以帮助你分析 GPU 的性能瓶颈，查看着色器的输出等。
