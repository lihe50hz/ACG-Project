#pragma once
#include "sparkium/scene/camera.h"
#include "sparkium/utils/utils.h"

namespace sparkium {
class CameraController {
 public:
  CameraController(vulkan::Core *core, Camera *camera);
  bool Update(float delta_time);

 private:
  vulkan::Core *core_;
  Camera *camera_;

  double last_x, last_y;
};
}  // namespace sparkium
