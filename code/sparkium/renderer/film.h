#pragma once

#include "sparkium/renderer/renderer_utils.h"

namespace sparkium {

constexpr VkFormat kAlbedoFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
constexpr VkFormat kPositionFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
constexpr VkFormat kNormalFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
constexpr VkFormat kRadianceFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
constexpr VkFormat kDepthFormat = VK_FORMAT_D32_SFLOAT;
constexpr VkFormat kStencilFormat = VK_FORMAT_R32G32_UINT;
constexpr VkFormat kResultFormat = VK_FORMAT_R32G32B32A32_SFLOAT;

struct Film {
  std::unique_ptr<vulkan::Image> albedo_image{};
  std::unique_ptr<vulkan::Image> position_image{};
  std::unique_ptr<vulkan::Image> normal_image{};
  std::unique_ptr<vulkan::Image> radiance_image{};
  std::unique_ptr<vulkan::Image> depth_image{};
  std::unique_ptr<vulkan::Image> stencil_image{};
  std::unique_ptr<vulkan::Image> result_image{};
  std::unique_ptr<vulkan::Framebuffer> framebuffer{};
  std::unique_ptr<vulkan::DescriptorPool> descriptor_pool{};
  std::unique_ptr<vulkan::DescriptorSet> lighting_attachment_set{};
  std::unique_ptr<vulkan::DescriptorSet> post_process_attachment_set{};
  Renderer *renderer{};

  void Resize(uint32_t width, uint32_t height);
};
}  // namespace sparkium
