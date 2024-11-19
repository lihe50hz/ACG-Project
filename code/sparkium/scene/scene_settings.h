#pragma once
#include "glm/glm.hpp"

namespace sparkium {

struct SceneSettings {
  glm::mat4 projection;      // camera_to_clip
  glm::mat4 inv_projection;  // inverse_projection
  glm::mat4 view;            // world_to_camera
  glm::mat4 inv_view;        // camera_to_world
  float gamma{2.2f};
  float exposure{1.0f};
  float persistence{1.0f};
  uint32_t accumulated_sample{0};
  uint32_t num_sample{16};
  uint32_t num_bounces{32};
  float clamp_value{10.0f};
  float total_emission_energy{0.0f};
  uint32_t num_entity{0};
  uint32_t enable_direct_lighting{0};
  float padding[6];
};  // need align to 64(0x40) byte

}  // namespace sparkium
