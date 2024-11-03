#version 460
#extension GL_EXT_ray_tracing : enable

#include "ray_payload.glsl"

layout(location = 0) rayPayloadInEXT RayPayload ray_payload;
hitAttributeEXT vec3 attribs;

void main() {
  ray_payload.t = gl_HitTEXT;
  if (ray_payload.t != -1.0) {
    ray_payload.barycentric =
        vec3(1.0 - attribs.x - attribs.y, attribs.x, attribs.y);
    ray_payload.entity_id = gl_InstanceCustomIndexEXT;
    ray_payload.primitive_id = gl_PrimitiveID;
    ray_payload.entity_transform = gl_ObjectToWorldEXT;
  } else {
    ray_payload.t = -1.0;
    ray_payload.entity_transform = mat4x3(1.0);
    ray_payload.barycentric = vec3(0.0);
    ray_payload.entity_id = 0xffffffffu;
    ray_payload.primitive_id = 0xffffffffu;
  }
}
