#include "sparkium/scene/scene.h"

#include "Eigen/Eigen"
#include "sparkium/renderer/renderer.h"

namespace sparkium {
Scene::Scene(struct Renderer *renderer, int max_entities)
    : renderer_(renderer) {
  vulkan::DescriptorPoolSize pool_size;
  pool_size = pool_size + renderer_->SceneDescriptorSetLayout()->GetPoolSize() *
                              renderer_->Core()->MaxFramesInFlight() * 2;

  pool_size =
      pool_size + renderer_->EnvmapDescriptorSetLayout()->GetPoolSize() *
                      renderer_->Core()->MaxFramesInFlight();

  pool_size =
      pool_size + renderer_->EntityDescriptorSetLayout()->GetPoolSize() *
                      max_entities * renderer_->Core()->MaxFramesInFlight();

  pool_size =
      pool_size + renderer_->RayTracingDescriptorSetLayout()->GetPoolSize() *
                      renderer_->Core()->MaxFramesInFlight();

  renderer_->Core()->Device()->CreateDescriptorPool(
      pool_size, renderer_->Core()->MaxFramesInFlight() * (max_entities + 4),
      &descriptor_pool_);

  scene_settings_buffer_ =
      std::make_unique<vulkan::DynamicBuffer<SceneSettings>>(
          renderer_->Core(), 2, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

  entity_material_buffer_ = std::make_unique<vulkan::DynamicBuffer<Material>>(
      renderer_->Core(), max_entities, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

  entity_metadata_buffer_ =
      std::make_unique<vulkan::DynamicBuffer<EntityMetadata>>(
          renderer_->Core(), max_entities, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

  descriptor_sets_.resize(renderer_->Core()->MaxFramesInFlight());
  for (int i = 0; i < renderer_->Core()->MaxFramesInFlight(); i++) {
    descriptor_pool_->AllocateDescriptorSet(
        renderer_->SceneDescriptorSetLayout()->Handle(), &descriptor_sets_[i]);

    descriptor_sets_[i]->BindUniformBuffer(
        0, scene_settings_buffer_->GetBuffer(i), 0, sizeof(SceneSettings));
    descriptor_sets_[i]->BindStorageBuffer(
        1, entity_material_buffer_->GetBuffer(i));
    descriptor_sets_[i]->BindStorageBuffer(
        2, entity_metadata_buffer_->GetBuffer(i));
  }
  far_descriptor_sets_.resize(renderer_->Core()->MaxFramesInFlight());
  for (int i = 0; i < renderer_->Core()->MaxFramesInFlight(); i++) {
    descriptor_pool_->AllocateDescriptorSet(
        renderer_->SceneDescriptorSetLayout()->Handle(),
        &far_descriptor_sets_[i]);

    far_descriptor_sets_[i]->BindUniformBuffer(
        0, scene_settings_buffer_->GetBuffer(i), sizeof(SceneSettings),
        sizeof(SceneSettings));
    far_descriptor_sets_[i]->BindStorageBuffer(
        1, entity_material_buffer_->GetBuffer(i));
    far_descriptor_sets_[i]->BindStorageBuffer(
        2, entity_metadata_buffer_->GetBuffer(i));
  }

  renderer_->Core()->CreateTopLevelAccelerationStructure({}, &top_level_as_);

  raytracing_descriptor_sets_.resize(renderer_->Core()->MaxFramesInFlight());
  for (int i = 0; i < renderer_->Core()->MaxFramesInFlight(); i++) {
    descriptor_pool_->AllocateDescriptorSet(
        renderer_->RayTracingDescriptorSetLayout()->Handle(),
        &raytracing_descriptor_sets_[i]);
  }

  envmap_ = std::make_unique<EnvMap>(this);
}

Scene::~Scene() {
  top_level_as_.reset();
  envmap_.reset();
  entities_.clear();
  descriptor_sets_.clear();
  far_descriptor_sets_.clear();
  raytracing_descriptor_sets_.clear();
  entity_material_buffer_.reset();
  entity_metadata_buffer_.reset();
  scene_settings_buffer_.reset();
  descriptor_pool_.reset();
}

int Scene::CreateEntity(Entity **pp_entity) {
  entities_[next_entity_id_] = std::make_unique<Entity>(this);
  if (pp_entity) {
    *pp_entity = entities_[next_entity_id_].get();
  }
  entities_[next_entity_id_]->metadata_.entity_id = next_entity_id_;
  return next_entity_id_++;
}

void Scene::Update(float delta_time) {
  if (update_callback_ && scene_settings_.persistence != 1.0f) {
    update_callback_(this, delta_time);
  }
}

void Scene::UpdatePipelineObjects() {
  envmap_->Update();
  UpdateDynamicBuffers();
  UpdateTopLevelAccelerationStructure();
  UpdateDescriptorSetBindings();
}

void Scene::SyncData(VkCommandBuffer cmd_buffer, int frame_id) {
  envmap_->Sync(cmd_buffer, frame_id);
  scene_settings_buffer_->SyncData(cmd_buffer, frame_id);
  for (auto &entity : entities_) {
    entity.second->Sync(cmd_buffer, frame_id);
  }
  entity_metadata_buffer_->SyncData(cmd_buffer, frame_id);
  entity_material_buffer_->SyncData(cmd_buffer, frame_id);
}

void Scene::UpdateDynamicBuffers() {
  Renderer()->AssetManager()->GetMeshIds();
  Renderer()->AssetManager()->GetTextureIds();

  for (auto &entity : entities_) {
    entity.second->Update();
  }

  float total_energy = 0.0f;

  for (auto &[id, entity] : entities_) {
    float energy = 0.0f;

    auto material = entity->GetMaterial();
    float energy_density = 0.0f;

    glm::vec3 emission = material.emission * material.emission_strength;
    energy_density = std::max(emission.r, std::max(emission.g, emission.b));

    if (energy_density > 0.0) {
      auto mesh = renderer_->AssetManager()->GetMesh(entity->MeshId());
      auto transform = glm::mat3(entity->GetTransform());

      Eigen::Matrix3<float> svd_transform;
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          svd_transform(i, j) = transform[i][j];
        }
      }

      Eigen::JacobiSVD<Eigen::Matrix3<float>> svd(
          svd_transform, Eigen::ComputeFullU | Eigen::ComputeFullV);

      Eigen::Vector3<float> singular_values = svd.singularValues();
      singular_values = singular_values.cwiseAbs().derived();

      std::sort(singular_values.data(),
                singular_values.data() + singular_values.size());

      float singular_value_0 = singular_values[2];
      float singular_value_1 = singular_values[1];

      float stretched_area = singular_value_0 * singular_value_1 * mesh->area_;
      energy = stretched_area * energy_density;
    }
    total_energy += energy;
    entity->SetEmissionCDF(total_energy);
  }
  scene_settings_.total_emission_energy = total_energy;

  uint32_t binding_entity_id = 0;
  for (auto &[id, entity] : entities_) {
    if (total_energy > 0.0) {
      entity->SetEmissionCDF(entity->GetEmissionCDF() / total_energy);
    } else {
      entity->SetEmissionCDF(0.0);
    }
    entity_metadata_buffer_->At(binding_entity_id) =
        entity->GetTranslatedMetadata();
    entity_material_buffer_->At(binding_entity_id) = entity->GetMaterial();
    binding_entity_id++;
  }
  scene_settings_.num_entity = entities_.size();

  VkExtent2D extent = renderer_->Core()->Swapchain()->Extent();
  SceneSettings scene_settings = scene_settings_;
  scene_settings.view = camera_.GetView();
  scene_settings.projection = camera_.GetProjection(
      static_cast<float>(extent.width) / static_cast<float>(extent.height));
  scene_settings.inv_projection = glm::inverse(scene_settings.projection);
  scene_settings.inv_view = glm::inverse(scene_settings.view);
  scene_settings_buffer_->At(0) = scene_settings;
  scene_settings.projection = camera_.GetProjectionFar(
      static_cast<float>(extent.width) / static_cast<float>(extent.height));
  scene_settings.inv_projection = glm::inverse(scene_settings.projection);
  scene_settings_buffer_->At(1) = scene_settings;
}

void Scene::UpdateTopLevelAccelerationStructure() {
  std::vector<std::pair<vulkan::AccelerationStructure *, glm::mat4>> instances;
  for (auto &[id, entity] : entities_) {
    uint32_t mesh_id = entity->MeshId();
    auto mesh = renderer_->AssetManager()->GetMesh(mesh_id);
    instances.emplace_back(mesh->blas_.get(), entity->metadata_.transform);
  }
  static int last_num_instances = -1;
  if (last_num_instances != instances.size()) {
    last_num_instances = instances.size();
    renderer_->Core()->Device()->WaitIdle();
    renderer_->Core()->CreateTopLevelAccelerationStructure(instances,
                                                           &top_level_as_);
  } else {
    top_level_as_->UpdateInstances(instances,
                                   renderer_->Core()->GraphicsCommandPool(),
                                   renderer_->Core()->GraphicsQueue());
  }
}

void Scene::UpdateDescriptorSetBindings() {
  uint32_t frame_id = renderer_->Core()->CurrentFrame();
  raytracing_descriptor_sets_[frame_id]->BindAccelerationStructure(
      0, top_level_as_.get());

  for (auto &[id, entity] : entities_) {
    entity->descriptor_sets_[frame_id]->BindCombinedImageSampler(
        2, renderer_->AssetManager()
               ->GetTexture(entity->metadata_.albedo_texture_id)
               ->image_.get());
    entity->descriptor_sets_[frame_id]->BindCombinedImageSampler(
        3, renderer_->AssetManager()
               ->GetTexture(entity->metadata_.albedo_detail_texture_id)
               ->image_.get());
  }
}

void Scene::DrawEnvmap(VkCommandBuffer cmd_buffer, int frame_id) {
  VkDescriptorSet descriptor_sets[] = {envmap_->DescriptorSet(frame_id)};
  vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          renderer_->EnvmapPipelineLayout()->Handle(), 1, 1,
                          descriptor_sets, 0, nullptr);
  vkCmdDraw(cmd_buffer, 6, 1, 0, 0);
}

void Scene::DrawEntities(VkCommandBuffer cmd_buffer, int frame_id) {
  for (auto &entity : entities_) {
    VkDescriptorSet descriptor_sets[] = {
        entity.second->DescriptorSet(frame_id)->Handle()};
    vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            renderer_->EntityPipelineLayout()->Handle(), 1, 1,
                            descriptor_sets, 0, nullptr);

    uint32_t mesh_id = entity.second->MeshId();
    auto mesh = renderer_->AssetManager()->GetMesh(mesh_id);
    VkBuffer vertex_buffers[] = {
        mesh->vertex_buffer_->GetBuffer(frame_id)->Handle()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cmd_buffer, 0, 1, vertex_buffers, offsets);
    vkCmdBindIndexBuffer(cmd_buffer,
                         mesh->index_buffer_->GetBuffer(frame_id)->Handle(), 0,
                         VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd_buffer, mesh->index_buffer_->Length(), 1, 0, 0, 0);
  }
}

int Scene::SetEntityTransform(uint32_t entity_id, const glm::mat4 &transform) {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  entities_[entity_id]->metadata_.transform = transform;
  return 0;
}

int Scene::GetEntityTransform(uint32_t entity_id, glm::mat4 &transform) const {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  transform = entities_.at(entity_id)->metadata_.transform;
  return 0;
}

int Scene::SetEntityMaterial(uint32_t entity_id, const Material &material) {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  entities_[entity_id]->material_ = material;
  return 0;
}

int Scene::GetEntityMaterial(uint32_t entity_id, Material &material) const {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  material = entities_.at(entity_id)->material_;
  return 0;
}

int Scene::SetEntityAlbedoTexture(uint32_t entity_id, uint32_t texture_id) {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  entities_[entity_id]->metadata_.albedo_texture_id = texture_id;
  return 0;
}

int Scene::GetEntityAlbedoTexture(uint32_t entity_id,
                                  uint32_t &texture_id) const {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  texture_id = entities_.at(entity_id)->metadata_.albedo_texture_id;
  return 0;
}

int Scene::SetEntityAlbedoDetailTexture(uint32_t entity_id,
                                        uint32_t texture_id) {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  entities_[entity_id]->metadata_.albedo_detail_texture_id = texture_id;
  return 0;
}

int Scene::GetEntityAlbedoDetailTexture(uint32_t entity_id,
                                        uint32_t &texture_id) const {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  texture_id = entities_.at(entity_id)->metadata_.albedo_detail_texture_id;
  return 0;
}

int Scene::SetEntityNormalTexture(uint32_t entity_id, uint32_t texture_id) {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  entities_[entity_id]->metadata_.normal_texture_id = texture_id;
  return 0;
}

int Scene::GetEntityNormalTexture(uint32_t entity_id,
                                  uint32_t texture_id) const {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  texture_id = entities_.at(entity_id)->metadata_.normal_texture_id;
  return 0;
}

int Scene::SetEntityDetailScaleOffset(uint32_t entity_id,
                                      const glm::vec4 &scale_offset) {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  entities_[entity_id]->metadata_.detail_scale_offset = scale_offset;
  return 0;
}

int Scene::GetEntityDetailScaleOffset(uint32_t entity_id,
                                      glm::vec4 &scale_offset) const {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  scale_offset = entities_.at(entity_id)->metadata_.detail_scale_offset;
  return 0;
}

int Scene::SetEntityMesh(uint32_t entity_id, uint32_t mesh_id) {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  entities_[entity_id]->metadata_.mesh_id = mesh_id;
  return 0;
}

int Scene::GetEntityMesh(uint32_t entity_id, uint32_t &mesh_id) const {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  mesh_id = entities_.at(entity_id)->metadata_.mesh_id;
  return 0;
}

int Scene::SetEnvmapSettings(const EnvMapSettings &settings) {
  envmap_->settings_ = settings;
  return 0;
}

int Scene::GetEnvmapSettings(EnvMapSettings &settings) const {
  settings = envmap_->settings_;
  return 0;
}

void Scene::SetSceneSettings(const SceneSettings &settings) {
  scene_settings_ = settings;
}

void Scene::GetSceneSettings(SceneSettings &settings) const {
  settings = scene_settings_;
}

int Scene::SetEntityMetadata(uint32_t entity_id,
                             const EntityMetadata &metadata) {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  entities_[entity_id]->metadata_ = metadata;
  return 0;
}

int Scene::GetEntityMetadata(uint32_t entity_id,
                             EntityMetadata &metadata) const {
  if (entities_.find(entity_id) == entities_.end()) {
    return -1;
  }
  metadata = entities_.at(entity_id)->metadata_;
  return 0;
}

}  // namespace sparkium
