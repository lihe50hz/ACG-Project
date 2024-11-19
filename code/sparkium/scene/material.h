#pragma once
#include "sparkium/scene/scene_utils.h"

namespace sparkium {

// Definition of material type ids
constexpr uint32_t MATERIAL_TYPE_LAMBERTIAN = 0;
constexpr uint32_t MATERIAL_TYPE_SPECULAR = 1;
constexpr uint32_t MATERIAL_TYPE_PRINCIPLED = 2;

// clang-format off
struct Material {
  glm::vec3 base_color{1.0f};
  float subsurface{0.0f};

  glm::vec3 subsurface_radius{1.0f};
  float metallic{0.0f};

  glm::vec3 subsurface_color{1.0f};
  float specular{0.0f};

  float specular_tint{0.0f};
  float roughness{0.05f};
  float anisotropic{0.0f};
  float anisotropic_rotation{0.0f};

  float sheen{0.0f};
  float sheen_tint{0.0f};
  float clearcoat{1.0f};
  float clearcoat_roughness{0.0f};

  float ior{1.00f};
  float transmission{0.0f};
  float transmission_roughness{0.0f};
  float emission_strength{0.0f};

  glm::vec3 emission{1.0f};
  float alpha{1.0f};

  glm::vec3 normal{0.5f, 0.5f, 1.0f};
  uint32_t type{0};

  // This structure needs to be padded to 16 bytes
};
// clang-format on

}  // namespace sparkium
