#ifndef ENTITY_METADATA_GLSL
#define ENTITY_METADATA_GLSL

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

#endif
