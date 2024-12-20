#pragma once

#include "sparkium/renderer/renderer_utils.h"

namespace sparkium {
struct RayTracingFilm {
  std::unique_ptr<vulkan::Image> accumulated_radiance_image{};
  std::unique_ptr<vulkan::Image> accumulated_weight_image{};
  std::unique_ptr<vulkan::Image> accumulated_phase_gb_image{};
  std::unique_ptr<vulkan::Image> raw_result_image{};
  std::unique_ptr<vulkan::Image> result_image{};
  std::unique_ptr<vulkan::DescriptorPool> descriptor_pool{};
  std::unique_ptr<vulkan::DescriptorSet> descriptor_set{};
  Renderer *renderer{};

  void Resize(uint32_t width, uint32_t height);

  void ClearAccumulationBuffer();
};
}  // namespace sparkium
