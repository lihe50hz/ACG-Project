#version 450

#include "entity_metadata.glsl"
#include "material.glsl"
#include "scene_settings.glsl"

layout(set = 0, binding = 0, std140) uniform SceneSettingsUniform {
  SceneSettings scene_settings;
};

layout(set = 1, binding = 0, std140) uniform EntityMetadataUniform {
  EntityMetadata metadata;
};

layout(set = 1, binding = 1, std140) uniform EntityMaterialUniform {
  Material material;
};

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_tangent;
layout(location = 3) in vec2 in_tex_coord;
layout(location = 4) in float in_signal;

layout(location = 0) out vec3 out_pos;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec3 out_tangent;
layout(location = 3) out vec3 out_bitangent;
layout(location = 4) out vec2 out_tex_coord;
layout(location = 5) out uint out_instance_id;

void main() {
  out_pos = vec3(metadata.model * vec4(in_pos, 1.0));
  out_normal = transpose(inverse(mat3(metadata.model))) * in_normal;
  out_tangent = mat3(metadata.model) * in_tangent;
  out_bitangent =
      mat3(metadata.model) * (in_signal * cross(in_normal, in_tangent));
  out_tex_coord = in_tex_coord;
  out_instance_id = gl_InstanceIndex;
  gl_Position =
      (scene_settings.projection * scene_settings.view * vec4(out_pos, 1.0)) *
      vec4(1.0, -1.0, 1.0, 1.0);
}
