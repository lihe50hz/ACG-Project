#pragma once
#include "sparkium/scene/scene_utils.h"

namespace sparkium {

struct EnvMapSettings {
  float offset{0.0f};
  float scale{1.0f};
  uint32_t envmap_id{0};
  int reflect{0};
};

class EnvMap {
 public:
  EnvMap(Scene *scene);
  ~EnvMap();

  EnvMapSettings Settings() const {
    return settings_;
  }

  VkDescriptorSet DescriptorSet(int frame_id) {
    return descriptor_sets_[frame_id]->Handle();
  }

  void SetEnvmapTexture(uint32_t envmap_id);

  void Update();

  void Sync(VkCommandBuffer cmd_buffer, int frame_id);

 private:
  friend Scene;
  Scene *scene_{nullptr};
  EnvMapSettings settings_{};
  std::unique_ptr<vulkan::DynamicBuffer<EnvMapSettings>>
      envmap_settings_buffer_{};
  std::vector<std::unique_ptr<vulkan::DescriptorSet>> descriptor_sets_{};
};
}  // namespace sparkium
