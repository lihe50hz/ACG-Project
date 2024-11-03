#pragma once

#include "glm/glm.hpp"
#include "sparkium/utils/utils.h"

namespace sparkium {
struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec3 tangent{};
  glm::vec2 tex_coord{};
  float signal{1.0f};

  bool operator<(const Vertex &vertex) const {
    if (position.x < vertex.position.x)
      return true;
    if (position.x > vertex.position.x)
      return false;
    if (position.y < vertex.position.y)
      return true;
    if (position.y > vertex.position.y)
      return false;
    if (position.z < vertex.position.z)
      return true;
    if (position.z > vertex.position.z)
      return false;
    if (normal.x < vertex.normal.x)
      return true;
    if (normal.x > vertex.normal.x)
      return false;
    if (normal.y < vertex.normal.y)
      return true;
    if (normal.y > vertex.normal.y)
      return false;
    if (normal.z < vertex.normal.z)
      return true;
    if (normal.z > vertex.normal.z)
      return false;
    if (tangent.x < vertex.tangent.x)
      return true;
    if (tangent.x > vertex.tangent.x)
      return false;
    if (tangent.y < vertex.tangent.y)
      return true;
    if (tangent.y > vertex.tangent.y)
      return false;
    if (tangent.z < vertex.tangent.z)
      return true;
    if (tangent.z > vertex.tangent.z)
      return false;
    if (tex_coord.x < vertex.tex_coord.x)
      return true;
    if (tex_coord.x > vertex.tex_coord.x)
      return false;
    if (tex_coord.y < vertex.tex_coord.y)
      return true;
    if (tex_coord.y > vertex.tex_coord.y)
      return false;
    return signal < vertex.signal;
  }

  bool operator==(const Vertex &vertex) const {
    return position == vertex.position && normal == vertex.normal &&
           tex_coord == tex_coord && tangent == vertex.tangent &&
           signal == vertex.signal;
  }

  friend std::ostream &operator<<(std::ostream &os, const Vertex &vertex) {
    os << "Vertex: {"
       << "position: " << vertex.position.x << ", " << vertex.position.y << ", "
       << vertex.position.z << ", "
       << "normal: " << vertex.normal.x << ", " << vertex.normal.y << ", "
       << vertex.normal.z << ", "
       << "tangent: " << vertex.tangent.x << ", " << vertex.tangent.y << ", "
       << vertex.tangent.z << ", "
       << "tex_coord: " << vertex.tex_coord.x << ", " << vertex.tex_coord.y
       << ", "
       << "signal: " << vertex.signal << "}";
    return os;
  }
};
}  // namespace sparkium
