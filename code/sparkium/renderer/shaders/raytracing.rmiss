#version 460
#extension GL_EXT_ray_tracing : enable

#include "ray_payload.glsl"

layout(location = 0) rayPayloadInEXT RayPayload ray_payload;

void main() {
  ray_payload.t = -1.0;
  ray_payload.entity_transform = mat4x3(1.0);
  ray_payload.barycentric = vec3(0.0);
  ray_payload.entity_id = 0xffffffffu;
  ray_payload.primitive_id = 0xffffffffu;
}
