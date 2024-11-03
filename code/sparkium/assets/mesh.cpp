#include "sparkium/assets/mesh.h"

#include "map"
#include "mikktspace.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace sparkium {

Mesh::Mesh(const std::vector<Vertex> &vertices,
           const std::vector<uint32_t> &indices)
    : vertices_(vertices), indices_(indices) {
  BuildTangent();
}

void Mesh::MergeVertices() {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  std::map<Vertex, uint32_t> vertex_index_map;
  auto index_func = [&vertices, &vertex_index_map](const Vertex &v) {
    if (vertex_index_map.count(v)) {
      return vertex_index_map.at(v);
    }
    uint32_t res = vertices.size();
    vertex_index_map[v] = res;
    vertices.push_back(v);
    return res;
  };
  for (auto ind : indices_) {
    indices.push_back(index_func(vertices_[ind]));
  }
  vertices_ = vertices;
  indices_ = indices;
}

void Mesh::BuildNormal() {
  std::vector<bool> face_count(vertices_.size(), false);

  for (int i = 0; i < vertices_.size(); i++) {
    glm::vec3 normal = vertices_[i].normal;
    if (glm::length(normal) < 0.5f) {
      face_count[i] = true;
    }
  }

  for (int face_i = 0; face_i < indices_.size() / 3; face_i++) {
    bool count_u = face_count[indices_[face_i * 3]];
    bool count_v = face_count[indices_[face_i * 3 + 1]];
    bool count_w = face_count[indices_[face_i * 3 + 2]];

    if (count_u || count_v || count_w) {
      Vertex &u = vertices_[indices_[face_i * 3]];
      Vertex &v = vertices_[indices_[face_i * 3 + 1]];
      Vertex &w = vertices_[indices_[face_i * 3 + 2]];

      glm::vec3 normal = glm::normalize(
          glm::cross(v.position - u.position, w.position - u.position));

      if (glm::isnan(normal.x) || glm::isnan(normal.y) ||
          glm::isnan(normal.z)) {
        continue;
      }

      if (count_u) {
        u.normal += normal;
      }

      if (count_v) {
        v.normal += normal;
      }

      if (count_w) {
        w.normal += normal;
      }
    }
  }

  for (int i = 0; i < vertices_.size(); i++) {
    if (face_count[i]) {
      vertices_[i].normal = glm::normalize(vertices_[i].normal);
    }
  }
}

void Mesh::BuildTangent() {
  if (!indices_.size() || !vertices_.size()) {
    return;
  }

  BuildNormal();

  std::vector<Vertex> vertices(indices_.size());
  for (int i = 0; i < indices_.size(); i++) {
    vertices[i] = vertices_[indices_[i]];
    indices_[i] = i;
  }
  vertices_ = vertices;

  SMikkTSpaceInterface interface {};

  interface.m_getNumFaces = [](const SMikkTSpaceContext *context) {
    auto mesh = reinterpret_cast<Mesh *>(context->m_pUserData);
    return int(mesh->indices_.size() / 3);
  };

  interface.m_getNumVerticesOfFace = [](const SMikkTSpaceContext *context,
                                        const int i_face) { return 3; };

  interface.m_getNormal = [](const SMikkTSpaceContext *context,
                             float normal_out[], const int i_face,
                             const int i_vert) {
    auto mesh = reinterpret_cast<Mesh *>(context->m_pUserData);
    auto normal = mesh->vertices_[mesh->indices_[i_face * 3 + i_vert]].normal;
    normal_out[0] = normal.x;
    normal_out[1] = normal.y;
    normal_out[2] = normal.z;
  };

  interface.m_getPosition = [](const SMikkTSpaceContext *context,
                               float position_out[], const int i_face,
                               const int i_vert) {
    auto mesh = reinterpret_cast<Mesh *>(context->m_pUserData);
    auto position =
        mesh->vertices_[mesh->indices_[i_face * 3 + i_vert]].position;
    position_out[0] = position.x;
    position_out[1] = position.y;
    position_out[2] = position.z;
  };

  interface.m_getTexCoord = [](const SMikkTSpaceContext *context,
                               float texcoord_out[], const int i_face,
                               const int i_vert) {
    auto mesh = reinterpret_cast<Mesh *>(context->m_pUserData);
    auto tex_coord =
        mesh->vertices_[mesh->indices_[i_face * 3 + i_vert]].tex_coord;
    texcoord_out[0] = tex_coord.x;
    texcoord_out[1] = tex_coord.y;
  };

  interface.m_setTSpaceBasic = [](const SMikkTSpaceContext *context,
                                  const float tangent[], const float sign,
                                  const int i_face, const int i_vert) {
    auto mesh = reinterpret_cast<Mesh *>(context->m_pUserData);
    auto &vert = mesh->vertices_[i_face * 3 + i_vert];
    vert.tangent = glm::vec3{tangent[0], tangent[1], tangent[2]};
    vert.signal = sign;
  };

  SMikkTSpaceContext context{};
  context.m_pInterface = &interface;
  context.m_pUserData = reinterpret_cast<void *>(this);

  if (!genTangSpaceDefault(&context)) {
    LogWarning("Build MikkTSpace failed.");
  }

  for (auto &vertex : vertices_) {
    if (std::abs(glm::dot(vertex.normal, vertex.tangent)) > 1e-4f) {
      vertex.tangent = glm::cross(vertex.normal, glm::vec3{1.0f, 0.0f, 0.0f});
      if (glm::length(vertex.tangent) < 1e-4f) {
        vertex.tangent = glm::cross(vertex.normal, glm::vec3{0.0f, 1.0f, 0.0f});
      }
      vertex.tangent = glm::normalize(vertex.tangent);
    }
  }

  MergeVertices();
}

int Mesh::LoadObjFile(const std::string &obj_file_path) {
  tinyobj::ObjReaderConfig reader_config;
  reader_config.mtl_search_path = "./";  // Path to material files

  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(obj_file_path, reader_config)) {
    if (!reader.Error().empty()) {
      LogWarning("[Load obj, ERROR]: {}", reader.Error());
    }
    return -1;
  }

  if (!reader.Warning().empty()) {
    LogWarning("{}", reader.Warning());
  }

  auto &attrib = reader.GetAttrib();
  auto &shapes = reader.GetShapes();
  auto &materials = reader.GetMaterials();

  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

      // Loop over vertices in the face.
      std::vector<Vertex> face_vertices;
      for (size_t v = 0; v < fv; v++) {
        Vertex vertex{};
        // access to vertex
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
        tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
        tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
        tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
        vertex.position = {vx, vy, vz};
        // Check if `normal_index` is zero or positive. negative = no normal
        // data
        if (idx.normal_index >= 0) {
          tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
          tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
          tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
          vertex.normal = {nx, ny, nz};
        } else {
          vertex.normal = {0.0f, 0.0f, 0.0f};
        }

        // Check if `texcoord_index` is zero or positive. negative = no texcoord
        // data
        if (idx.texcoord_index >= 0) {
          tinyobj::real_t tx =
              attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
          tinyobj::real_t ty =
              attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
          vertex.tex_coord = {tx, ty};
        }
        face_vertices.push_back(vertex);
      }

      for (int i = 1; i < face_vertices.size() - 1; i++) {
        Vertex v0 = face_vertices[0];
        Vertex v1 = face_vertices[i];
        Vertex v2 = face_vertices[i + 1];
        auto geometry_normal = glm::normalize(
            glm::cross(v1.position - v0.position, v2.position - v0.position));
        if (v0.normal == glm::vec3{0.0f, 0.0f, 0.0f}) {
          v0.normal = geometry_normal;
        } else if (glm::dot(geometry_normal, v0.normal) < 0.0f) {
          v0.normal = -v0.normal;
        }
        if (v1.normal == glm::vec3{0.0f, 0.0f, 0.0f}) {
          v1.normal = geometry_normal;
        } else if (glm::dot(geometry_normal, v1.normal) < 0.0f) {
          v1.normal = -v1.normal;
        }
        if (v2.normal == glm::vec3{0.0f, 0.0f, 0.0f}) {
          v2.normal = geometry_normal;
        } else if (glm::dot(geometry_normal, v2.normal) < 0.0f) {
          v2.normal = -v2.normal;
        }
        indices.push_back(vertices.size());
        indices.push_back(vertices.size() + 1);
        indices.push_back(vertices.size() + 2);
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
      }

      index_offset += fv;
    }
  }
  vertices_ = vertices;
  indices_ = indices;
  BuildTangent();
  return 0;
}

int Mesh::SaveObjFile(const std::string &obj_file_path) const {
  std::ofstream file(obj_file_path);
  if (!file.is_open()) {
    LogWarning("Failed to open file: {}", obj_file_path);
    return -1;
  }

  for (const auto &vertex : vertices_) {
    file << "v " << vertex.position.x << " " << vertex.position.y << " "
         << vertex.position.z << std::endl;
  }
  for (const auto &vertex : vertices_) {
    file << "vn " << vertex.normal.x << " " << vertex.normal.y << " "
         << vertex.normal.z << std::endl;
  }
  for (const auto &vertex : vertices_) {
    file << "vt " << vertex.tex_coord.x << " " << vertex.tex_coord.y
         << std::endl;
  }
  for (size_t i = 0; i < indices_.size(); i += 3) {
    file << "f " << indices_[i] + 1 << "/" << indices_[i] + 1 << "/"
         << indices_[i] + 1 << " " << indices_[i + 1] + 1 << "/"
         << indices_[i + 1] + 1 << "/" << indices_[i + 1] + 1 << " "
         << indices_[i + 2] + 1 << "/" << indices_[i + 2] + 1 << "/"
         << indices_[i + 2] + 1 << std::endl;
  }
  file.close();
  return 0;
}

int Mesh::LoadFromHeightMap(const Texture &height_map,
                            float precision,
                            float height_scale,
                            float height_offset) {
  int width = height_map.Width() * precision;
  int height = height_map.Height() * precision;
  float inv_width = 1.0f / width;
  float inv_height = 1.0f / height;
  glm::vec3 scale = {inv_width, height_scale, inv_height};
  std::vector<Vertex> vertices((width + 1) * (height + 1), Vertex{});
  std::vector<uint32_t> indices(width * height * 6, 0);

  for (int i = 0; i <= width; i++) {
    for (int j = 0; j <= height; j++) {
      glm::vec4 color =
          height_map.Sample((float(i)) * inv_width, (float(j)) * inv_height,
                            AddressMode::BlackBorder);
      float h = color.r * 0.299f + color.g * 0.587f + color.b * 0.114f;
      vertices[j * (width + 1) + i].position =
          glm::vec3{float(i), h, float(j)} * scale +
          glm::vec3{0.0f, height_offset, 0.0f};
      vertices[j * (width + 1) + i].tex_coord = {float(i) * inv_width,
                                                 float(j) * inv_height};
    }
  }

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      indices[(j * width + i) * 6] = j * (width + 1) + i;
      indices[(j * width + i) * 6 + 1] = (j + 1) * (width + 1) + i;
      indices[(j * width + i) * 6 + 2] = j * (width + 1) + i + 1;
      indices[(j * width + i) * 6 + 3] = (j + 1) * (width + 1) + i + 1;
      indices[(j * width + i) * 6 + 4] = j * (width + 1) + i + 1;
      indices[(j * width + i) * 6 + 5] = (j + 1) * (width + 1) + i;
    }
  }

  CalculateNormals(vertices, indices);

  vertices_ = vertices;
  indices_ = indices;
  BuildTangent();

  return 0;
}

void CalculateNormals(std::vector<Vertex> &vertices,
                      const std::vector<uint32_t> &indices) {
  std::vector<glm::vec3> normals(vertices.size(), glm::vec3{0.0f});
  std::vector<float> weights(vertices.size(), 0.0f);
  for (int i = 0; i < indices.size(); i += 3) {
    glm::vec3 v0 = vertices[indices[i]].position;
    glm::vec3 v1 = vertices[indices[i + 1]].position;
    glm::vec3 v2 = vertices[indices[i + 2]].position;
    glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);
    float weight = glm::length(normal);
    for (int j = 0; j < 3; j++) {
      normals[indices[i + j]] += normal;
      weights[indices[i + j]] += weight;
    }
  }

  for (int i = 0; i < vertices.size(); i++) {
    vertices[i].normal = glm::normalize(normals[i] / weights[i]);
  }
}
}  // namespace sparkium
