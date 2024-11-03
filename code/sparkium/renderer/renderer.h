#pragma once

#include "sparkium/asset_manager/asset_manager.h"
#include "sparkium/renderer/film.h"
#include "sparkium/renderer/raytracing_film.h"
#include "sparkium/renderer/renderer_utils.h"
#include "sparkium/scene/scene.h"

namespace sparkium {
class Renderer {
 public:
  Renderer(class AssetManager *asset_manager);

  ~Renderer();

  vulkan::Core *Core() {
    return core_;
  }

  class AssetManager *AssetManager() {
    return asset_manager_;
  }

  int CreateScene(int max_entities, double_ptr<class Scene> pp_scene);

  vulkan::DescriptorSetLayout *SceneDescriptorSetLayout() {
    return scene_descriptor_set_layout_.get();
  }

  vulkan::DescriptorSetLayout *EnvmapDescriptorSetLayout() {
    return envmap_descriptor_set_layout_.get();
  }

  vulkan::DescriptorSetLayout *EntityDescriptorSetLayout() {
    return entity_descriptor_set_layout_.get();
  }

  vulkan::DescriptorSetLayout *RayTracingDescriptorSetLayout() {
    return raytracing_descriptor_set_layout_.get();
  }

  vulkan::DescriptorSetLayout *LightingDescriptorSetLayout() {
    return lighting_descriptor_set_layout_.get();
  }

  vulkan::DescriptorSetLayout *PostProcessDescriptorSetLayout() {
    return post_process_descriptor_set_layout_.get();
  }

  vulkan::DescriptorSetLayout *RayTracingFilmDescriptorSetLayout() {
    return raytracing_film_descriptor_set_layout_.get();
  }

  vulkan::Pipeline *EntityPipeline() {
    return entity_pipeline_.get();
  }

  vulkan::Pipeline *EnvmapPipeline() {
    return envmap_pipeline_.get();
  }

  vulkan::PipelineLayout *EntityPipelineLayout() {
    return entity_pipeline_layout_.get();
  }

  vulkan::PipelineLayout *EnvmapPipelineLayout() {
    return envmap_pipeline_layout_.get();
  }

  vulkan::PipelineLayout *LightingPipelineLayout() {
    return lighting_pipeline_layout_.get();
  }

  vulkan::PipelineLayout *PostProcessPipelineLayout() {
    return post_process_pipeline_layout_.get();
  }

  vulkan::RenderPass *RenderPass() {
    return render_pass_.get();
  }

  vulkan::Sampler *DefaultSampler() {
    return sampler_.get();
  }

  int CreateFilm(uint32_t width, uint32_t height, double_ptr<Film> pp_film);

  int CreateRayTracingFilm(uint32_t width,
                           uint32_t height,
                           double_ptr<RayTracingFilm> pp_film);

  void RenderScene(VkCommandBuffer cmd_buffer, Film *film, Scene *scene);

  void RenderSceneRayTracing(VkCommandBuffer cmd_buffer,
                             RayTracingFilm *film,
                             Scene *scene);

 private:
  void CreateCommonObjects();

  void CreateRenderPass();

  void CreateEnvmapPipeline();

  void CreateEntityPipeline();

  void CreateLightingPipeline();

  void CreatePostProcessPipeline();

  void CreateRayTracingPipeline();

  void DestroyCommonObjects();

  void DestroyRenderPass();

  void DestroyEnvmapPipeline();

  void DestroyEntityPipeline();

  void DestroyLightingPipeline();

  void DestroyPostProcessPipeline();

  void DestroyRayTracingPipeline();

  vulkan::Core *core_{};

  class AssetManager *asset_manager_{};

  std::unique_ptr<vulkan::DescriptorSetLayout> scene_descriptor_set_layout_;
  std::unique_ptr<vulkan::RenderPass> render_pass_;

  std::unique_ptr<vulkan::DescriptorSetLayout> entity_descriptor_set_layout_;
  std::unique_ptr<vulkan::PipelineLayout> entity_pipeline_layout_;
  std::unique_ptr<vulkan::ShaderModule> entity_vertex_shader_;
  std::unique_ptr<vulkan::ShaderModule> entity_fragment_shader_;
  std::unique_ptr<vulkan::Pipeline> entity_pipeline_;

  std::unique_ptr<vulkan::DescriptorSetLayout> envmap_descriptor_set_layout_;
  std::unique_ptr<vulkan::PipelineLayout> envmap_pipeline_layout_;
  std::unique_ptr<vulkan::ShaderModule> envmap_vertex_shader_;
  std::unique_ptr<vulkan::ShaderModule> envmap_fragment_shader_;
  std::unique_ptr<vulkan::Pipeline> envmap_pipeline_;

  std::unique_ptr<vulkan::DescriptorSetLayout> lighting_descriptor_set_layout_;
  std::unique_ptr<vulkan::PipelineLayout> lighting_pipeline_layout_;

  std::unique_ptr<vulkan::DescriptorSetLayout>
      post_process_descriptor_set_layout_;
  std::unique_ptr<vulkan::PipelineLayout> post_process_pipeline_layout_;
  std::unique_ptr<vulkan::ShaderModule> post_process_vertex_shader_;
  std::unique_ptr<vulkan::ShaderModule> post_process_fragment_shader_;
  std::unique_ptr<vulkan::Pipeline> post_process_pipeline_;

  std::unique_ptr<vulkan::DescriptorSetLayout>
      raytracing_descriptor_set_layout_;
  std::unique_ptr<vulkan::DescriptorSetLayout>
      raytracing_film_descriptor_set_layout_;
  std::unique_ptr<vulkan::PipelineLayout> raytracing_pipeline_layout_;
  std::unique_ptr<vulkan::ShaderModule> raytracing_raygen_shader_;
  std::unique_ptr<vulkan::ShaderModule> raytracing_miss_shader_;
  std::unique_ptr<vulkan::ShaderModule> raytracing_closest_hit_shader_;
  std::unique_ptr<vulkan::Pipeline> raytracing_pipeline_;
  std::unique_ptr<vulkan::ShaderBindingTable> raytracing_sbt_;

  std::unique_ptr<vulkan::Sampler> sampler_;
};
}  // namespace sparkium
