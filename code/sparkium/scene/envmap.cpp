#include "sparkium/scene/envmap.h"

#include "sparkium/renderer/renderer.h"
#include "sparkium/scene/scene.h"

namespace sparkium {

EnvMap::EnvMap(Scene *scene) : scene_(scene) {
  envmap_settings_buffer_ =
      std::make_unique<vulkan::DynamicBuffer<EnvMapSettings>>(
          scene_->Renderer()->Core(), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

  descriptor_sets_.resize(scene_->Renderer()->Core()->MaxFramesInFlight());

  auto asset_manager = scene_->Renderer()->AssetManager();

  for (size_t i = 0; i < descriptor_sets_.size(); ++i) {
    scene_->DescriptorPool()->AllocateDescriptorSet(
        scene_->Renderer()->EnvmapDescriptorSetLayout()->Handle(),
        &descriptor_sets_[i]);
    descriptor_sets_[i]->BindUniformBuffer(
        0, envmap_settings_buffer_->GetBuffer(i));
    auto texture = asset_manager->GetTexture(0);
    descriptor_sets_[i]->BindCombinedImageSampler(1, texture->image_.get());
  }
}

EnvMap::~EnvMap() {
  descriptor_sets_.clear();
  envmap_settings_buffer_.reset();
}

void EnvMap::Update() {
  EnvMapSettings settings = settings_;
  settings.envmap_id = scene_->Renderer()->AssetManager()->GetTextureBindingId(
      settings.envmap_id);
  envmap_settings_buffer_->At(0) = settings;
  descriptor_sets_[scene_->Renderer()->Core()->CurrentFrame()]
      ->BindCombinedImageSampler(1, scene_->Renderer()
                                        ->AssetManager()
                                        ->GetTexture(settings_.envmap_id)
                                        ->image_.get());
  descriptor_sets_[scene_->Renderer()->Core()->CurrentFrame()]
      ->BindStorageBuffer(2, scene_->Renderer()
                                 ->AssetManager()
                                 ->GetTexture(settings_.envmap_id)
                                 ->cdf_buffer_->GetBuffer());
}

void EnvMap::Sync(VkCommandBuffer cmd_buffer, int frame_id) {
  envmap_settings_buffer_->SyncData(cmd_buffer, frame_id);
}

void EnvMap::SetEnvmapTexture(uint32_t envmap_id) {
  settings_.envmap_id = envmap_id;
}

}  // namespace sparkium
