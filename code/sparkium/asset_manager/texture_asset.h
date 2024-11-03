#pragma once
#include "sparkium/asset_manager/asset_manager_utils.h"

namespace sparkium {
struct TextureAsset {
  std::unique_ptr<vulkan::Image> image_;
  std::unique_ptr<vulkan::StaticBuffer<float>> cdf_buffer_;
  std::string name_;
};
}  // namespace sparkium
