#ifndef VERTEX_GLSL
#define VERTEX_GLSL

#define VERTEX_VAR_COUNT 12

struct Vertex {
  vec3 position;
  vec3 normal;
  vec3 tangent;
  vec2 tex_coord;
  float signal;
};

Vertex GetVertex(uint mesh_id, uint vertex_id) {
  uint offset = vertex_id * VERTEX_VAR_COUNT;
  Vertex vertex;
  vertex.position = vec3(vertex_buffers[mesh_id].vertex_data[offset + 0],
                         vertex_buffers[mesh_id].vertex_data[offset + 1],
                         vertex_buffers[mesh_id].vertex_data[offset + 2]);
  vertex.normal = vec3(vertex_buffers[mesh_id].vertex_data[offset + 3],
                       vertex_buffers[mesh_id].vertex_data[offset + 4],
                       vertex_buffers[mesh_id].vertex_data[offset + 5]);
  vertex.tangent = vec3(vertex_buffers[mesh_id].vertex_data[offset + 6],
                        vertex_buffers[mesh_id].vertex_data[offset + 7],
                        vertex_buffers[mesh_id].vertex_data[offset + 8]);
  vertex.tex_coord = vec2(vertex_buffers[mesh_id].vertex_data[offset + 9],
                          vertex_buffers[mesh_id].vertex_data[offset + 10]);
  vertex.signal = vertex_buffers[mesh_id].vertex_data[offset + 11];
  return vertex;
}

vec3 GetVertexPos(uint mesh_id, uint vertex_id) {
  uint offset = vertex_id * VERTEX_VAR_COUNT;
  return vec3(vertex_buffers[mesh_id].vertex_data[offset + 0],
              vertex_buffers[mesh_id].vertex_data[offset + 1],
              vertex_buffers[mesh_id].vertex_data[offset + 2]);
}

#endif
