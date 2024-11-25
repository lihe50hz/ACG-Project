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
  uint32_t num_sample{1};
  uint32_t num_bounces{16};
  float clamp_value{20.0f};
  float total_emission_energy{0.0f};
  uint32_t num_entity{0};
  uint32_t enable_direct_lighting{1};
  uint32_t enable_volumetric_rendering{0};

  // for depth of field
  float aperture{0};            // the aperture of the lens
  float focal_distance{500};    // the distance from the focus plane to the lens

  float padding1[3];

  // for motion blur
  glm::vec3 camera_velocity{0.0f};    // the velocity of the camera
  float blur_decay{1.0f};             // the decay for motion blur

  float padding2[12];
};  // need align to 64(0x40) byte

}  // namespace sparkium
