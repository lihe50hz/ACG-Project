#pragma once

#include "sparkium/assets/texture.h"
#include "sparkium/assets/vertex.h"
#include "vector"

namespace sparkium {
class Mesh {
 public:
  Mesh(const std::vector<Vertex> &vertices = {},
       const std::vector<uint32_t> &indices = {});

  int LoadObjFile(const std::string &obj_file_path);

  int SaveObjFile(const std::string &obj_file_path) const;

  int LoadFromHeightMap(const Texture &height_map,
                        float precision = 1.0f,
                        float height_scale = 1.0f,
                        float height_offset = 0.0f);

  const std::vector<Vertex> &Vertices() const {
    return vertices_;
  }

  const std::vector<uint32_t> &Indices() const {
    return indices_;
  }

  void ThisIsPointLight(glm::vec3 shift = glm::vec3{-131.452f, -114.514f, -191.981f}) {
    for (auto &vertex : vertices_)
      vertex.position = vertex.position + shift;
  }

  void Shift(glm::vec3 shift) {
    for (auto &vertex : vertices_)
      vertex.position = vertex.position + shift;
  }

 private:
  void MergeVertices();

  void BuildNormal();

  void BuildTangent();

  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;
};

void CalculateNormals(std::vector<Vertex> &vertices,
                      const std::vector<uint32_t> &indices);
}  // namespace sparkium
