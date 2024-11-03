#version 450
#include "scene_settings.glsl"

layout(set = 0, binding = 0, std140) uniform SceneSettingsUniform {
  SceneSettings scene_settings;
};

const vec2[6] positions = vec2[6](vec2(-1.0, -1.0),
                                  vec2(1.0, -1.0),
                                  vec2(-1.0, 1.0),
                                  vec2(-1.0, 1.0),
                                  vec2(1.0, -1.0),
                                  vec2(1.0, 1.0));

layout(location = 0) out vec3 ray_direction;

void main() {
  gl_Position =
      vec4(positions[gl_VertexIndex], 0.0, 1.0) * vec4(1.0, -1.0, 0.0, 1.0);
  mat4 proj = scene_settings.projection;
  proj[0][0] = 1.0 / proj[0][0];
  proj[1][1] = 1.0 / proj[1][1];
  ray_direction =
      mat3(scene_settings.inv_view) *
      normalize(vec4(positions[gl_VertexIndex], 0.0, 1.0) * proj).xyz;
}
