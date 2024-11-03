#include "sparkium/scene/entity.h"

#include "sparkium/renderer/renderer.h"
#include "sparkium/scene/scene.h"

namespace sparkium {
Entity::Entity(Scene *scene) : scene_(scene) {
  material_buffer_ = std::make_unique<vulkan::DynamicBuffer<Material>>(
      scene_->Renderer()->Core(), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
  metadata_buffer_ = std::make_unique<vulkan::DynamicBuffer<EntityMetadata>>(
      scene_->Renderer()->Core(), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

  descriptor_sets_.resize(scene->Renderer()->Core()->MaxFramesInFlight());
  auto asset_manager = scene_->Renderer()->AssetManager();
  for (int i = 0; i < descriptor_sets_.size(); i++) {
    auto &descriptor_set = descriptor_sets_[i];
    scene_->DescriptorPool()->AllocateDescriptorSet(
        scene_->Renderer()->EntityDescriptorSetLayout()->Handle(),
        &descriptor_set);
    auto texture = asset_manager->GetTexture(0);
    descriptor_set->BindUniformBuffer(0, metadata_buffer_->GetBuffer(i));
    descriptor_set->BindUniformBuffer(1, material_buffer_->GetBuffer(i));
    descriptor_set->BindCombinedImageSampler(2, texture->image_.get());
    descriptor_set->BindCombinedImageSampler(3, texture->image_.get());
  }
}

Entity::~Entity() {
  descriptor_sets_.clear();
  material_buffer_.reset();
}

vulkan::Core *Entity::Core() const {
  return scene_->Renderer()->Core();
}

EntityMetadata Entity::GetTranslatedMetadata() const {
  AssetManager *asset_manager = scene_->Renderer()->AssetManager();
  EntityMetadata metadata = metadata_;
  metadata.albedo_texture_id =
      asset_manager->GetTextureBindingId(metadata_.albedo_texture_id);
  metadata.albedo_detail_texture_id =
      asset_manager->GetTextureBindingId(metadata_.albedo_detail_texture_id);
  metadata.mesh_id = asset_manager->GetMeshBindingId(metadata_.mesh_id);
  return metadata;
}

void Entity::Update() {
  metadata_buffer_->At(0) = metadata_;
  material_buffer_->At(0) = material_;
}

void Entity::Sync(VkCommandBuffer cmd_buffer, int frame_id) {
  metadata_buffer_->SyncData(cmd_buffer, frame_id);
  material_buffer_->SyncData(cmd_buffer, frame_id);
}
}  // namespace sparkium
