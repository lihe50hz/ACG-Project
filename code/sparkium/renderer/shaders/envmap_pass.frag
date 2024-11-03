#version 450

layout(location = 0) in vec3 ray_direction;

#define ENVMAP_SET 1
#include "envmap.glsl"

layout(set = ENVMAP_SET, binding = 1) uniform sampler2D envmap;

layout(location = 0) out vec4 out_albedo;
layout(location = 1) out vec4 out_position;
layout(location = 2) out vec4 out_normal;
layout(location = 3) out vec4 out_color;
layout(location = 4) out uvec4 out_instance;

void main() {
  vec3 direction = normalize(ray_direction);
  vec3 color =
      texture(envmap, SampleEnvmapUV(direction)).rgb * envmap_data.scale;
  out_albedo = vec4(0.0);

  out_position = vec4(0.0);
  out_normal = vec4(-direction, 0.0);
  out_color = vec4(color, 1.0);
  out_instance = uvec4(0xffffffffu, 0xffffffffu, 0, 0);
}
