#ifndef HIT_RECORD_GLSL
#define HIT_RECORD_GLSL

#include "entity_metadata.glsl"
#include "vertex.glsl"

struct HitRecord {
  uint entity_id;
  vec3 position;
  vec3 shading_normal;
  vec3 geometry_normal;
  vec3 tangent;
  vec3 bitangent;
  vec2 tex_coord;
  vec3 omega_v;
  bool front_face;

  uint albedo_texture_id;
  uint albedo_detail_texture_id;
  vec4 detail_scale_offset;
} hit_record;

HitRecord ComposeHitRecord(RayPayload ray_payload,
                           vec3 origin,
                           vec3 direction) {
  HitRecord hit_record;
  hit_record.entity_id = 0;
  hit_record.position = vec3(0.0);
  hit_record.shading_normal = vec3(0.0);
  hit_record.geometry_normal = vec3(0.0);
  hit_record.tangent = vec3(0.0);
  hit_record.bitangent = vec3(0.0);
  hit_record.tex_coord = vec2(0.0);
  hit_record.omega_v = -direction;
  hit_record.front_face = true;
  hit_record.albedo_texture_id = 0;
  hit_record.albedo_detail_texture_id = 0;
  hit_record.detail_scale_offset = vec4(1.0, 1.0, 0.0, 0.0);

  hit_record.entity_id = ray_payload.entity_id;
  EntityMetadata metadata = metadatas[hit_record.entity_id];
  hit_record.albedo_texture_id = metadata.albedo_texture_id;
  hit_record.albedo_detail_texture_id = metadata.albedo_detail_texture_id;
  hit_record.detail_scale_offset = metadata.detail_scale_offset;
  Vertex v0 = GetVertex(metadata.mesh_id,
                        index_buffers[metadata.mesh_id]
                            .indices[ray_payload.primitive_id * 3 + 0]);
  Vertex v1 = GetVertex(metadata.mesh_id,
                        index_buffers[metadata.mesh_id]
                            .indices[ray_payload.primitive_id * 3 + 1]);
  Vertex v2 = GetVertex(metadata.mesh_id,
                        index_buffers[metadata.mesh_id]
                            .indices[ray_payload.primitive_id * 3 + 2]);
  vec3 b0 = v0.signal * cross(v0.normal, v0.tangent);
  vec3 b1 = v1.signal * cross(v1.normal, v1.tangent);
  vec3 b2 = v2.signal * cross(v2.normal, v2.tangent);

  mat3 entity_transform = mat3(ray_payload.entity_transform);
  hit_record.position = ray_payload.entity_transform *
                        vec4(mat3(v0.position, v1.position, v2.position) *
                                 ray_payload.barycentric,
                             1.0);
  hit_record.shading_normal = normalize(transpose(inverse(entity_transform)) *
                                        mat3(v0.normal, v1.normal, v2.normal) *
                                        ray_payload.barycentric);
  hit_record.geometry_normal =
      normalize(transpose(inverse(entity_transform)) *
                cross(v1.position - v0.position, v2.position - v0.position));
  hit_record.tangent =
      normalize(entity_transform * mat3(v0.tangent, v1.tangent, v2.tangent) *
                ray_payload.barycentric);
  hit_record.bitangent =
      normalize(entity_transform * mat3(b0, b1, b2) * ray_payload.barycentric);
  hit_record.tex_coord = mat3x2(v0.tex_coord, v1.tex_coord, v2.tex_coord) *
                         ray_payload.barycentric;

  if (dot(hit_record.geometry_normal, hit_record.shading_normal) < 0.0) {
    hit_record.geometry_normal = -hit_record.geometry_normal;
  }

  if (dot(direction, hit_record.geometry_normal) > 0.0) {
    hit_record.front_face = false;
    hit_record.geometry_normal = -hit_record.geometry_normal;
    hit_record.shading_normal = -hit_record.shading_normal;
    hit_record.tangent = -hit_record.tangent;
    hit_record.bitangent = -hit_record.bitangent;
  }

  return hit_record;
}

#endif
