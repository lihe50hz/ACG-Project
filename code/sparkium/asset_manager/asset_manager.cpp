#include "sparkium/asset_manager/asset_manager.h"

#include <utility>

namespace sparkium {
AssetManager::AssetManager(vulkan::Core *core,
                           uint32_t max_textures,
                           uint32_t max_meshes)
    : core_(core), max_textures_(max_textures), max_meshes_(max_meshes) {
  CreateDescriptorObjects();
  CreateDefaultAssets();
}

AssetManager::~AssetManager() {
  DestroyDefaultAssets();
  DestroyDescriptorObjects();
}

void AssetManager::CreateDefaultAssets() {
  Texture white{1, 1, glm::vec4{1.0f}};
  LoadTexture(white, "Pure White");
  Mesh mesh;
  mesh.LoadObjFile(FindAssetsFile("mesh/cube.obj"));
  LoadMesh(mesh, "Cube");
}

void AssetManager::DestroyDefaultAssets() {
  textures_.clear();
  meshes_.clear();
}

void AssetManager::CreateDescriptorObjects() {
  core_->Device()->CreateSampler(
      VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT,
      VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_FALSE,
      VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK, VK_SAMPLER_MIPMAP_MODE_LINEAR,
      &linear_sampler_);

  core_->Device()->CreateSampler(
      VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT,
      VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_FALSE,
      VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK, VK_SAMPLER_MIPMAP_MODE_NEAREST,
      &nearest_sampler_);

  mesh_metadata_buffer_ = std::make_unique<vulkan::DynamicBuffer<MeshMetadata>>(
      core_, max_meshes_, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

  core_->Device()->CreateDescriptorSetLayout(
      {{0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, max_meshes_,
        VK_SHADER_STAGE_RAYGEN_BIT_KHR, nullptr},
       {1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, max_meshes_,
        VK_SHADER_STAGE_RAYGEN_BIT_KHR, nullptr},
       {2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, max_meshes_,
        VK_SHADER_STAGE_RAYGEN_BIT_KHR, nullptr},
       {3, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        nullptr},
       {4, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, max_textures_,
        VK_SHADER_STAGE_RAYGEN_BIT_KHR, nullptr},
       {5, VK_DESCRIPTOR_TYPE_SAMPLER, 2, VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        nullptr}},
      &descriptor_set_layout_);

  vulkan::DescriptorPoolSize pool_size =
      descriptor_set_layout_->GetPoolSize() * core_->MaxFramesInFlight();
  core_->Device()->CreateDescriptorPool(pool_size, core_->MaxFramesInFlight(),
                                        &descriptor_pool_);
  descriptor_sets_.resize(core_->MaxFramesInFlight());
  for (size_t frame_id = 0; frame_id < core_->MaxFramesInFlight(); frame_id++) {
    auto &descriptor_set = descriptor_sets_[frame_id];
    descriptor_pool_->AllocateDescriptorSet(descriptor_set_layout_->Handle(),
                                            &descriptor_set);
    core_->Device()->NameObject(
        descriptor_set->Handle(),
        fmt::format("Asset Manager Descriptor Set [{}]", frame_id));
  }

  for (size_t frame_id = 0; frame_id < core_->MaxFramesInFlight(); frame_id++) {
    auto &descriptor_set = descriptor_sets_[frame_id];
    descriptor_set->BindStorageBuffer(
        3, mesh_metadata_buffer_->GetBuffer(frame_id));
    descriptor_set->BindSamplers(
        5, {linear_sampler_->Handle(), nearest_sampler_->Handle()});
  }

  last_frame_bound_mesh_num_ =
      std::vector<uint32_t>(core_->MaxFramesInFlight(), max_meshes_);
  last_frame_bound_texture_num_ =
      std::vector<uint32_t>(core_->MaxFramesInFlight(), max_textures_);
}

void AssetManager::DestroyDescriptorObjects() {
  descriptor_sets_.clear();
  descriptor_pool_.reset();
  descriptor_set_layout_.reset();
  linear_sampler_.reset();
  nearest_sampler_.reset();
}

int AssetManager::LoadTexture(const Texture &texture, std::string name) {
  TextureAsset texture_asset;
  texture_asset.name_ = std::move(name);
  if (core_->Device()->CreateImage(
          VK_FORMAT_R32G32B32A32_SFLOAT,
          VkExtent2D{texture.Width(), texture.Height()},
          &texture_asset.image_) != VK_SUCCESS) {
    return -1;
  }

  UploadImage(core_->GraphicsQueue(), core_->GraphicsCommandPool(),
              texture_asset.image_.get(), texture.Data(),
              texture.Width() * texture.Height() * sizeof(glm::vec4));

  std::vector<float> pixel_cdf(texture.Width() * texture.Height());

  float accumulated_weight = 0.0;
  for (uint32_t y = 0; y < texture.Height(); y++) {
    for (uint32_t x = 0; x < texture.Width(); x++) {
      uint32_t index = y * texture.Width() + x;
      glm::vec3 pixel{texture(x, y)};
      accumulated_weight += std::max(pixel.x, std::max(pixel.y, pixel.z));
      pixel_cdf[index] = accumulated_weight;
    }
  }

  for (auto &weight : pixel_cdf) {
    weight /= accumulated_weight;
  }
  core_->CreateStaticBuffer<float>(pixel_cdf.size(),
                                   VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                   &texture_asset.cdf_buffer_);
  texture_asset.cdf_buffer_->UploadContents(pixel_cdf.data(), pixel_cdf.size());

  uint32_t binding_texture_id = textures_.size();

  textures_[next_texture_id_] = {
      binding_texture_id,
      std::make_unique<TextureAsset>(std::move(texture_asset))};

  return next_texture_id_++;
}

int AssetManager::LoadMesh(const Mesh &mesh, std::string name) {
  auto &vertices = mesh.Vertices();
  auto &indices = mesh.Indices();

  float area = 0.0;
  std::vector<float> area_cdf(indices.size() / 3);
  for (int i = 0; i < indices.size() / 3; i++) {
    area += glm::length(glm::cross(vertices[indices[i * 3 + 1]].position -
                                       vertices[indices[i * 3]].position,
                                   vertices[indices[i * 3 + 2]].position -
                                       vertices[indices[i * 3]].position)) *
            0.5;
    area_cdf[i] = area;
  }
  for (auto &weight : area_cdf) {
    weight /= area;
  }

  MeshAsset mesh_asset;
  mesh_asset.name_ = std::move(name);
  if (core_->CreateStaticBuffer<Vertex>(
          mesh.Vertices().size(),
          VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
              VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
              VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR |
              VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
          &mesh_asset.vertex_buffer_) != VK_SUCCESS) {
    return -1;
  }

  if (core_->CreateStaticBuffer<uint32_t>(
          mesh.Indices().size(),
          VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
              VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
              VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR |
              VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
          &mesh_asset.index_buffer_) != VK_SUCCESS) {
    return -1;
  }

  if (core_->CreateStaticBuffer<float>(
          area_cdf.size(), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
          &mesh_asset.area_cdf_buffer_) != VK_SUCCESS) {
    return -1;
  }

  mesh_asset.vertex_buffer_->UploadContents(mesh.Vertices().data(),
                                            mesh.Vertices().size());
  mesh_asset.index_buffer_->UploadContents(mesh.Indices().data(),
                                           mesh.Indices().size());
  mesh_asset.area_cdf_buffer_->UploadContents(area_cdf.data(), area_cdf.size());
  mesh_asset.area_ = area;

  if (core_->CreateBottomLevelAccelerationStructure(
          mesh_asset.vertex_buffer_->GetBuffer(),
          mesh_asset.index_buffer_->GetBuffer(), sizeof(Vertex),
          &mesh_asset.blas_) != VK_SUCCESS) {
    return -1;
  }

  uint32_t binding_mesh_id = meshes_.size();
  meshes_[next_mesh_id_] = {binding_mesh_id,
                            std::make_unique<MeshAsset>(std::move(mesh_asset))};

  return next_mesh_id_++;
}

void AssetManager::DestroyTexture(uint32_t id) {
  textures_.erase(id);
}

void AssetManager::DestroyMesh(uint32_t id) {
  meshes_.erase(id);
}

void AssetManager::ImGui() {
  if (ImGui::Begin("Asset Manager")) {
    ImGui::SeparatorText("Textures");
    for (const auto &[id, texture] : textures_) {
      ImGui::Text("%s", texture.second->name_.c_str());
    }

    ImGui::SeparatorText("Meshes");
    for (const auto &[id, mesh] : meshes_) {
      ImGui::Text("%s", mesh.second->name_.c_str());
    }
  }
  ImGui::End();
}

TextureAsset *AssetManager::GetTexture(uint32_t id) {
  if (textures_.find(id) != textures_.end()) {
    return textures_[id].second.get();
  }
  return textures_[0].second.get();
}

MeshAsset *AssetManager::GetMesh(uint32_t id) {
  if (meshes_.find(id) != meshes_.end()) {
    return meshes_[id].second.get();
  }
  return meshes_[0].second.get();
}

uint32_t AssetManager::GetTextureBindingId(uint32_t id) {
  return textures_[id].first;
}

uint32_t AssetManager::GetMeshBindingId(uint32_t id) {
  return meshes_[id].first;
}

std::set<uint32_t> AssetManager::GetTextureIds() {
  std::set<uint32_t> ids;
  uint32_t binding_texture_ids = 0;
  for (auto &[id, texture] : textures_) {
    ids.insert(id);
    texture.first = binding_texture_ids++;
  }
  return ids;
}

std::set<uint32_t> AssetManager::GetMeshIds() {
  std::set<uint32_t> ids;
  uint32_t binding_mesh_ids = 0;
  for (auto &[id, mesh] : meshes_) {
    ids.insert(id);
    mesh.first = binding_mesh_ids++;
  }
  return ids;
}

void AssetManager::UpdateMeshDataBindings(uint32_t frame_id) {
  auto &descriptor_set = descriptor_sets_[frame_id];

  std::vector<const vulkan::Buffer *> vertex_buffers;
  std::vector<const vulkan::Buffer *> index_buffers;
  std::vector<const vulkan::Buffer *> area_cdf_buffers;
  for (auto mesh_id : GetMeshIds()) {
    auto mesh = GetMesh(mesh_id);
    vertex_buffers.push_back(mesh->vertex_buffer_->GetBuffer(frame_id));
    index_buffers.push_back(mesh->index_buffer_->GetBuffer(frame_id));
    area_cdf_buffers.push_back(mesh->area_cdf_buffer_->GetBuffer(frame_id));
  }

  uint32_t last_frame_bound_mesh_num = last_frame_bound_mesh_num_[frame_id];
  last_frame_bound_mesh_num_[frame_id] = vertex_buffers.size();
  if (last_frame_bound_mesh_num != vertex_buffers.size()) {
    while (vertex_buffers.size() < last_frame_bound_mesh_num ||
           index_buffers.size() < last_frame_bound_mesh_num ||
           area_cdf_buffers.size() < last_frame_bound_mesh_num) {
      auto mesh = GetMesh(0);
      vertex_buffers.push_back(mesh->vertex_buffer_->GetBuffer(frame_id));
      index_buffers.push_back(mesh->index_buffer_->GetBuffer(frame_id));
      area_cdf_buffers.push_back(mesh->area_cdf_buffer_->GetBuffer(frame_id));
    }
  }

  descriptor_set->BindStorageBuffers(0, vertex_buffers);
  descriptor_set->BindStorageBuffers(1, index_buffers);
  descriptor_set->BindStorageBuffers(2, area_cdf_buffers);

  for (int i = 0; i < vertex_buffers.size(); i++) {
    MeshMetadata metadata;
    metadata.num_vertex = GetMesh(i)->vertex_buffer_->Length();
    metadata.num_index = GetMesh(i)->index_buffer_->Length();
    mesh_metadata_buffer_->At(i) = metadata;
  }
}

void AssetManager::UpdateTextureBindings(uint32_t frame_id) {
  std::vector<const vulkan::Image *> images;
  for (auto texture_id : GetTextureIds()) {
    auto texture = GetTexture(texture_id);
    images.push_back(texture->image_.get());
  }

  uint32_t last_frame_bound_texture_num =
      last_frame_bound_texture_num_[frame_id];
  last_frame_bound_texture_num_[frame_id] = images.size();
  if (last_frame_bound_texture_num != images.size()) {
    while (images.size() < last_frame_bound_texture_num) {
      auto texture = GetTexture(0);
      images.push_back(texture->image_.get());
    }
  }

  auto &descriptor_set = descriptor_sets_[frame_id];
  descriptor_set->BindSampledImages(4, images);
}

void AssetManager::Update(uint32_t frame_id) {
  UpdateMeshDataBindings(frame_id);
  UpdateTextureBindings(frame_id);
}

bool AssetManager::ComboForTextureSelection(const char *label, uint32_t *id) {
  bool result = false;
  std::vector<const char *> items;
  std::vector<uint32_t> item_ids;
  int current_selection = 0;
  for (auto &[item_id, item] : textures_) {
    items.push_back(item.second->name_.c_str());
    item_ids.push_back(item_id);
    if (item_id == *id) {
      current_selection = items.size() - 1;
    }
  }
  result = ImGui::Combo(label, &current_selection, items.data(),
                        static_cast<int>(items.size()));
  if (result) {
    *id = item_ids[current_selection];
  }
  return result;
}

bool AssetManager::ComboForMeshSelection(const char *label, uint32_t *id) {
  bool result = false;
  std::vector<const char *> items;
  std::vector<uint32_t> item_ids;
  int current_selection = 0;
  for (auto &[item_id, item] : meshes_) {
    items.push_back(item.second->name_.c_str());
    item_ids.push_back(item_id);
    if (item_id == *id) {
      current_selection = items.size() - 1;
    }
  }
  result = ImGui::Combo(label, &current_selection, items.data(),
                        static_cast<int>(items.size()));
  if (result) {
    *id = item_ids[current_selection];
  }
  return result;
}

void AssetManager::SyncData(VkCommandBuffer cmd_buffer, int frame_id) {
  mesh_metadata_buffer_->SyncData(cmd_buffer, frame_id);
}

void AssetManager::Clear() {
  next_mesh_id_ = 0;
  next_texture_id_ = 0;
  textures_.clear();
  meshes_.clear();
  last_frame_bound_texture_num_ =
      std::vector<uint32_t>(core_->MaxFramesInFlight(), max_textures_);
  last_frame_bound_mesh_num_ =
      std::vector<uint32_t>(core_->MaxFramesInFlight(), max_meshes_);
  CreateDefaultAssets();
}

}  // namespace sparkium
