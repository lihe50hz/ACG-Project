# 着色器资源定义

框架中已给出许多着色器资源定义，这些定义可以直接使用，也可以根据需要进行修改。

<!-- TOC -->
* [着色器资源定义](#着色器资源定义)
  * [Scene Set](#scene-set)
    * [SceneSettings](#scenesettings)
    * [Material](#material)
    * [EntityMetadata](#entitymetadata)
  * [Asset Manager Set](#asset-manager-set)
    * [Vertex](#vertex)
    * [Index](#index)
    * [Area CDF](#area-cdf)
    * [MeshMetadata](#meshmetadata)
    * [Textures & Samplers](#textures--samplers)
  * [Environment Set](#environment-set)
    * [Environment Map CDF](#environment-map-cdf)
<!-- TOC -->

## Scene Set

### SceneSettings

C++ 端的定义位于 [code/sparkium/scene/scene_settings.h](../code/sparkium/scene/scene_settings.h)

```glsl
struct SceneSettings {
    mat4 projection;
    mat4 inv_projection;
    mat4 view;
    mat4 inv_view;
    float gamma;
    float exposure;
    float persistence;
    uint accumulated_sample;
    uint num_sample;
    uint num_bounces;
    float clamp_value;
    float total_emission_energy;
    uint num_entity;
    bool enable_direct_lighting;
};
```

- project：投影矩阵，基于 Camera 的参数计算得到，可以用于完成投影变换。在光线追踪管线中没有被使用，主要在光栅化预览管线中使用。
- inv_project：投影矩阵，用于将屏幕坐标转换为初始光线的方向。这部分逻辑已经在框架中实现，不需要额外处理。
- view：观察矩阵，基于 Camera 的参数计算得到，可以用于完成观察变换。在光线追踪管线中没有被使用，主要在光栅化预览管线中使用。
- inv_view：观察矩阵的逆矩阵，用于将世界坐标转换为相机坐标。这部分逻辑已经在框架中实现，不需要额外处理。
- gamma：屏幕显示的 gamma 值，用于 gamma 校正。已在框架中实现，不需要额外处理。
- exposure：曝光值，用于调整图像的亮度。已在框架中实现，不需要额外处理。
- persistence：持续性，即每次追踪的光线的能量衰减系数，用于实时查看光追结果时降低噪声。当持续性为 1 时，每次追踪的光线的能量不会衰减，此时的光线追踪器为渲染模式，控制场景变化的场景更新函数停止调用，场景会定格以获得更高质量的图像。
- accumulated_sample：已累积的样本数，用于控制随机种子的生成。已在框架中实现，不需要额外处理。
- num_sample：当次着色器调用每个像素的样本数，用于控制单词渲染调用的任务量。已在框架中实现，不需要额外处理。
- num_bounces：光线追踪的最大反射次数，用于控制光线追踪的递归深度。
- clamp_value：光线追踪的能量的上限值，用于控制极端采样值的影响。
- total_emission_energy：场景中所有光源的总辐射能量，用于计算光源的能量分布。
- num_entity：场景中实体的数量，用于确定实体的索引。
- enable_direct_lighting：是否启用直接光照，用于控制光线追踪的光照模型。bool 类型在 GLSL 中同样占用 4 字节，因此在 C++ 中需要使用 uint 类型对应。

### Material

```glsl
struct Material {
  vec3 base_color;
  float subsurface;

  vec3 subsurface_radius;
  float metallic;

  vec3 subsurface_color;
  float specular;

  float specular_tint;
  float roughness;
  float anisotropic;
  float anisotropic_rotation;

  float sheen;
  float sheen_tint;
  float clearcoat;
  float clearcoat_roughness;

  float ior;
  float transmission;
  float transmission_roughness;
  float emission_strength;

  vec3 emission;
  float alpha;

  vec3 normal;
  uint type;
};
```

C++ 端的定义位于 [code/sparkium/scene/material.h](../code/sparkium/scene/material.h)

- type：材质类型，目前预定义了 Lambertian，Specular 和 Principled BSDF 三种材质代码。
  - 材质代码见 [material.h](../code/sparkium/scene/material.h) 中的宏定义
  - Lambertian：漫反射材质
  - Specular：镜面反射材质
  - Principled BSDF：基于物理的渲染材质，请自行查找相关资料学习相关定义和实现
- base_color：基础颜色，物体的主要颜色。又称反照率（albedo）。
  - Lambertian 材质和 Specular 材质中理论上只需要用到这一项，但 Principled BSDF 材质中可能会用到其他项。
- subsurface：次表面散射，定义光线进入物体内部进行散射的比例。
- subsurface_radius：次表面散射半径，三个分量分别定义了红、绿、蓝通道的次表面散射半径。
- metallic：金属度，定义物体表面的金属性程度。
- specular：高光强度，定义物体表面的高光程度。
- specular_tint：高光色调，定义高光的颜色与 Base Color 的贴合程度。
- roughness：粗糙度，定义物体表面的粗糙程度。
- anisotropic：各向异性，定义物体表面的各向异性程度。
- anisotropic_rotation：各向异性旋转，定义各向异性的旋转程度。
- sheen：光泽，定义物体表面的光泽程度。
- sheen_tint：光泽色调，定义光泽的颜色。
- clearcoat：透明面漆，定义物体表面的透明面漆程度。
- clearcoat_roughness：透明面漆粗糙度，定义透明面漆的粗糙程度。
- ior：折射率，定义物体的折射率。
- transmission：透射度，定义物体的透射程度。
- transmission_roughness：透射粗糙度，定义透射的粗糙程度。
- emission：自发光，定义物体的自发光颜色。
- emission_strength：自发光强度，定义物体的自发光强度。
- alpha：透明度，定义物体的透明度，即光线发生交互的概率。当值为 0 时，物体完全透明，当值为 1 时，物体完全不透明。

### EntityMetadata

```glsl
struct EntityMetadata {
  mat4 model;
  uint entity_id;
  uint mesh_id;
  uint albedo_texture_id;
  uint albedo_detail_texture_id;
  vec4 detail_scale_offset;
  float emission_cdf;
};
```

C++ 端的定义位于 [code/sparkium/scene/entity.h](../code/sparkium/scene/entity.h)

- model：模型矩阵，用于将物体从模型空间转换到世界空间。
- entity_id：实体 ID，用于确定实体的索引。主要用于在预览管线中确定实体的索引。在光线追踪管线中没有被使用。
- mesh_id：网格 ID，用于确定实体的网格。
  - GLSL 中读取到的 Mesh ID 为绑定到着色器的资源索引，而在 C++ 中的 Mesh ID 为 AssetManager 中注册的索引。
  - 在传递到 GLSL 着色器之前，框架已经将 Mesh ID 转换为 GLSL 着色器中的绑定索引。
  - 具体过程见 `Entity::GetTranslatedMetadata` 函数中的实现以及框架中的相关调用。
- albedo_texture_id：反照率纹理 ID，用于确定实体的反照率纹理。
- albedo_detail_texture_id：反照率细节纹理 ID，用于确定实体的反照率细节纹理。
  - GLSL 中读取到的 Texture ID 同样为绑定到着色器的资源索引，而在 C++ 中的 Texture ID 为 AssetManager 中注册的索引。
  - 在传递到 GLSL 着色器之前，框架已经将 Texture ID 转换为 GLSL 着色器中的绑定索引。
  - 具体过程同样见 `Entity::GetTranslatedMetadata` 函数中的实现以及框架中的相关调用。
- detail_scale_offset：细节纹理的缩放和偏移，用于调整细节纹理的显示效果。前两个分量为缩放，后两个分量为偏移。
- emission_cdf：自发光累积分布函数，服务于光源直接采样（Direct Lighting）。
  - 保证按照 Entity ID 顺序非严格递增，即每个 Entity 的被采样概率应为其 `emission_cdf` 与按照编号前一个 Entity 的 `emission_cdf` 的差，具体的数值为框架建议的采样分布数值，可以参考 `Scene::UpdateDynamicBuffers` 函数中的实现。
  - 你也可以按照自己的需求重新定义采样分布数值。
  - 当存在 Entity 有自发光时，第一个（编号为0的） Entity 的 `emission_cdf` 即为其被采样的概率。最后一个 Entity 的 `emission_cdf` 保证为 1，即保证所有 Entity 被采样的概率之和为 1。
  - 当没有 Entity 有自发光时，`emission_cdf` 为 0。

## Asset Manager Set

### Vertex

```glsl
struct Vertex {
  vec3 position;
  vec3 normal;
  vec3 tangent;
  vec2 tex_coord;
  float signal;
};
```

Vertex 在 GLSL 中的绑定方式略有特殊，因为 Vertex Buffer 需要同时在光栅化管线中被调用，以及出于节约资源的考虑，Vertex 结构体不能按照 GLSL 的标准方式进行对齐占位。
所以在绑定资源时，GLSL 端按照 float 数组的方式进行解读，然后通过 [vertex.glsl](../code/sparkium/renderer/shaders/vertex.glsl) 中的 `GetVertex` 函数进行解析。

C++ 端的定义位于 [code/sparkium/assets/vertex.h](../code/sparkium/assets/vertex.h)

- position：顶点位置。
- normal：顶点法线。
- tangent：顶点切线。
- tex_coord：顶点纹理坐标。
- signal：二重切线 (Bitangent) 相对于 normal 叉乘 tangent 的方向，用于确定切线空间的正负方向。

### Index

`index_buffers[mesh_id].indices[primitive_id * 3 + 0]`,
`index_buffers[mesh_id].indices[primitive_id * 3 + 1]`,
`index_buffers[mesh_id].indices[primitive_id * 3 + 2]` 表示编号为 `mesh_id` 的 Mesh 的编号为 `primitive_id` 的三角形的三个顶点的索引。

### Area CDF

用于光源直接采样（Direct Lighting）的，对每个三角网格中三角形元素的面积累积分布函数。每个三角形的面积的概率正比于其面积。计算过程见 `AssetManager::LoadMesh` 函数中的实现。

`mesh_cdf_buffers[mesh_id].area_cdfs[primitive_id]` 表示编号为 `mesh_id` 的 Mesh 的编号为 `primitive_id` 的三角形的面积累积分布函数值。


### MeshMetadata

```glsl
struct MeshMetadata {
  uint num_vertex;
  uint num_index;
};
```

C++ 端的定义位于 [code/sparkium/asset_manager/mesh_asset.h](../code/sparkium/asset_manager/mesh_asset.h) 中。

- num_vertex：网格的顶点数量。
- num_index：网格的索引数量。（注意：是索引数量，而不是三角形数量）

### Textures & Samplers

```glsl
layout(set = 2, binding = 4) uniform texture2D sampled_textures[];
layout(set = 2, binding = 5) uniform sampler samplers[];
```

以上两行代码定义了在 GLSL 中使用的纹理和采样器数组。为了方便调用，框架中内置了如下两个函数用于分别进行两种不同插值方式的纹理采样：

```glsl
vec4 SampleTextureLinear(uint texture_id, vec2 uv); // 线性插值采样
vec4 SampleTextureNearest(uint texture_id, vec2 uv); // 最近邻插值采样
```
## Environment Set

### Environment Map CDF

用于针对光源进行直接采样（Direct Lighting），对环境贴图的每个像素的累积分布函数。每个像素的概率正比于其辐射能量。计算过程见 `AssetManager::LoadTexture` 函数中的实现。
数组的长度为环境贴图的像素数，即宽度乘以高度。
