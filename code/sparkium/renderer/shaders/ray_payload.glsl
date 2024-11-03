#ifndef RAY_PAYLOAD_GLSL
#define RAY_PAYLOAD_GLSL

struct RayPayload {
  mat4x3 entity_transform;
  vec3 barycentric;
  float t;
  uint entity_id;
  uint primitive_id;
};

#endif
