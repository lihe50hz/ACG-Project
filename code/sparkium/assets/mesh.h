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
