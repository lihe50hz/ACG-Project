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
  uint normal_texture_id;
  float padding0;
  float padding1;
  // align to 16 bytes
};

#endif
