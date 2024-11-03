#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include "sparkium/asset_manager/asset_manager.h"
#include "sparkium/scene/scene.h"

namespace sparkium {

void LoadCornellBox(Scene *scene, AssetManager *asset_manager) {
}

void LoadIslandScene(Scene *scene, AssetManager *asset_manager) {
  scene->Camera()->GetPosition() = glm::vec3{0.0f, 0.0f, 5.0f};

  auto envmap = scene->GetEnvMap();

  Texture envmap_texture;
  envmap_texture.LoadFromFile(FindAssetsFile("texture/envmap_clouds_4k.hdr"),
                              LDRColorSpace::UNORM);
  auto envmap_id = asset_manager->LoadTexture(envmap_texture, "Envmap");
  envmap->SetEnvmapTexture(envmap_id);
  scene->SetEnvmapSettings({0.0f, 1.0f, uint32_t(envmap_id), 0});

  int entity_id = scene->CreateEntity();

  Texture terrain_texture;
  terrain_texture.LoadFromFile(
      FindAssetsFile("texture/terrain/terrain-texture3.bmp"),
      LDRColorSpace::UNORM);

  Texture terrain_detail_texture;
  terrain_detail_texture.LoadFromFile(
      FindAssetsFile("texture/terrain/detail.bmp"), LDRColorSpace::UNORM);
  auto terrain_texture_id =
      asset_manager->LoadTexture(terrain_texture, "TerrainTexture");
  auto terrain_detail_texture_id = asset_manager->LoadTexture(
      terrain_detail_texture, "TerrainDetailTexture");

  scene->SetEntityAlbedoTexture(entity_id, terrain_texture_id);
  scene->SetEntityAlbedoDetailTexture(entity_id, terrain_detail_texture_id);

  Texture heightmap_texture;
  heightmap_texture.LoadFromFile(
      FindAssetsFile("texture/terrain/heightmap.bmp"), LDRColorSpace::UNORM);
  Mesh terrain_mesh;
  terrain_mesh.LoadFromHeightMap(heightmap_texture, 1.0f, 0.2f, 0.0f);
  auto terrain_mesh_id = asset_manager->LoadMesh(terrain_mesh, "TerrainMesh");
  Material terrain_material;
  terrain_material.sheen = 1.0f;
  scene->SetEntityMesh(entity_id, terrain_mesh_id);
  scene->SetEntityMaterial(entity_id, terrain_material);
  scene->SetEntityTransform(
      entity_id,
      glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, -0.06f, 0.0f}));
  scene->SetEntityDetailScaleOffset(entity_id, {20.0f, 20.0f, 0.0f, 0.0f});

  Mesh plane_mesh;
  std::vector<Vertex> plane_vertices;
  std::vector<uint32_t> plane_indices;
  const int precision = 500;
  const float inv_precision = 1.0f / static_cast<float>(precision);
  for (int i = 0; i <= precision; i++) {
    for (int j = 0; j <= precision; j++) {
      Vertex vertex;
      vertex.position = {static_cast<float>(i) * inv_precision - 0.5f, 0.0f,
                         static_cast<float>(j) * inv_precision - 0.5f};
      vertex.normal = {0.0f, 1.0f, 0.0f};
      vertex.tangent = {1.0f, 0.0f, 0.0f};
      vertex.tex_coord = {static_cast<float>(i) * inv_precision,
                          1.0f - static_cast<float>(j) * inv_precision};
      vertex.signal = 1.0f;
      plane_vertices.push_back(vertex);
    }
  }
  for (int i = 0; i < precision; i++) {
    for (int j = 0; j < precision; j++) {
      plane_indices.push_back(i * (precision + 1) + j);
      plane_indices.push_back(i * (precision + 1) + j + 1);
      plane_indices.push_back((i + 1) * (precision + 1) + j);
      plane_indices.push_back((i + 1) * (precision + 1) + j);
      plane_indices.push_back(i * (precision + 1) + j + 1);
      plane_indices.push_back((i + 1) * (precision + 1) + j + 1);
    }
  }
  plane_mesh = {plane_vertices, plane_indices};

  auto plane_mesh_id = asset_manager->LoadMesh(plane_mesh, "PlaneMesh");

  Texture water_texture;
  water_texture.LoadFromFile(
      FindAssetsFile("texture/terrain/SkyBox/SkyBox5.bmp"),
      LDRColorSpace::UNORM);
  auto water_texture_id =
      asset_manager->LoadTexture(water_texture, "WaterTexture");
  int water_entity_id = scene->CreateEntity();
  Material water_material;
  water_material.base_color = {1.0f, 1.0f, 1.0f};
  water_material.metallic = 0.1f;
  water_material.specular = 1.0f;
  water_material.alpha = 1.0f;
  scene->SetEntityAlbedoDetailTexture(water_entity_id, water_texture_id);
  scene->SetEntityMesh(water_entity_id, plane_mesh_id);
  scene->SetEntityMaterial(water_entity_id, water_material);
  scene->SetEntityTransform(water_entity_id,
                            glm::scale(glm::mat4{1.0f}, glm::vec3{1000.0f}));
  scene->SetEntityDetailScaleOffset(water_entity_id,
                                    {10000.0f, 10000.0f, 0.0f, 0.0f});

  scene->Camera()->SetFar(500.0f);
  scene->Camera()->SetNear(0.05f);
  scene->Camera()->SetPosition({0.0f, 0.1f, 1.2f});

  scene->SetUpdateCallback([=](Scene *scene, float delta_time) {
    glm::vec2 speed{0.3f, 1.0f};
    speed *= delta_time * 0.1f;
    glm::vec4 detail_scale_offset;
    scene->GetEntityDetailScaleOffset(water_entity_id, detail_scale_offset);
    detail_scale_offset.z += speed.x;
    detail_scale_offset.w += speed.y;
    detail_scale_offset.z = glm::mod(detail_scale_offset.z, 1.0f);
    detail_scale_offset.w = glm::mod(detail_scale_offset.w, 1.0f);
    scene->SetEntityDetailScaleOffset(water_entity_id, detail_scale_offset);
  });
}
}  // namespace sparkium
