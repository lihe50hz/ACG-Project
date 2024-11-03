#ifndef ENVMAP_GLSL
#define ENVMAP_GLSL

#include "constants.glsl"

layout(set = ENVMAP_SET, binding = 0, std140) uniform EnvmapData {
  float offset;
  float scale;
  int envmap_id;
  bool reflect;
}
envmap_data;

vec2 SampleEnvmapUV(vec3 direction) {
  direction.y = acos(direction.y) / PI;
  direction.x = atan(-direction.x, direction.z) / (2.0 * PI);

  if (envmap_data.reflect && direction.y > 0.5) {
    direction.y = 1.0 - direction.y;
  }
  direction.x += envmap_data.offset;
  direction.xy = mod(direction.xy, 1.0);

  return direction.xy;
}

vec3 SampleEnvmapDirection(vec2 uv) {
  uv = mod(uv, 1.0);
  uv.x -= envmap_data.offset;
  vec3 direction;
  direction.y = cos(uv.y * PI);
  float r = sin(uv.y * PI);
  direction.x = -r * sin(uv.x * 2.0 * PI);
  direction.z = r * cos(uv.x * 2.0 * PI);
  return direction;
}

#endif
