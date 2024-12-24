#include "sparkium/renderer/renderer.h"

#include "sparkium/scene/scene.h"

namespace sparkium {

namespace {

#include "built_in_shaders.inl"

}

Renderer::Renderer(class AssetManager *asset_manager)
    : core_(asset_manager->Core()), asset_manager_(asset_manager) {
  CreateCommonObjects();
  CreateRenderPass();
  CreateEnvmapPipeline();
  CreateEntityPipeline();
  CreateLightingPipeline();
  CreatePostProcessPipeline();
  CreateRayTracingPipeline();
}

Renderer::~Renderer() {
  DestroyRayTracingPipeline();
  DestroyPostProcessPipeline();
  DestroyLightingPipeline();
  DestroyEntityPipeline();
  DestroyEnvmapPipeline();
  DestroyRenderPass();
  DestroyCommonObjects();
}

void Renderer::CreateCommonObjects() {
  core_->Device()->CreateSampler(
      VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT,
      VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_FALSE,
      VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE, VK_SAMPLER_MIPMAP_MODE_LINEAR,
      &sampler_);

  VkSampler sampler = sampler_->Handle();

  core_->Device()->CreateDescriptorSetLayout(
      {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT |
            VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        nullptr},
       {1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        nullptr},
       {2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        nullptr}},
      &scene_descriptor_set_layout_);
}

void Renderer::DestroyCommonObjects() {
  scene_descriptor_set_layout_.reset();
  sampler_.reset();
}

void Renderer::CreateRenderPass() {
  std::vector<VkAttachmentDescription> descriptions;
  std::vector<vulkan::SubpassSettings> subpasses;
  std::vector<VkSubpassDependency> dependencies;
  descriptions.emplace_back(VkAttachmentDescription{
      0, kAlbedoFormat, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL});
  descriptions.emplace_back(VkAttachmentDescription{
      0, kPositionFormat, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL});
  descriptions.emplace_back(VkAttachmentDescription{
      0, kNormalFormat, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL});
  descriptions.emplace_back(VkAttachmentDescription{
      0, kRadianceFormat, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL});
  descriptions.emplace_back(VkAttachmentDescription{
      0, kDepthFormat, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL});
  descriptions.emplace_back(VkAttachmentDescription{
      0, kStencilFormat, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL});
  descriptions.emplace_back(VkAttachmentDescription{
      0, kResultFormat, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL});

  vulkan::SubpassSettings subpass;
  subpass.color_attachment_references.push_back(
      VkAttachmentReference{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
  subpass.color_attachment_references.push_back(
      VkAttachmentReference{1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
  subpass.color_attachment_references.push_back(
      VkAttachmentReference{2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
  subpass.color_attachment_references.push_back(
      VkAttachmentReference{3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
  subpass.color_attachment_references.push_back(
      VkAttachmentReference{5, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
  subpass.depth_attachment_reference = VkAttachmentReference{
      4, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};
  subpass.preserve_attachment_references = {6};
  subpasses.push_back(subpass);

  subpasses.emplace_back(
      std::vector<VkAttachmentReference>{
          {0, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL},
          {1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL},
          {2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL},
      },
      std::vector<VkAttachmentReference>{
          {3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL},
      },
      std::nullopt, std::vector<VkAttachmentReference>{},
      std::vector<uint32_t>{4, 5, 6});
  subpasses.emplace_back(
      std::vector<VkAttachmentReference>{
          {3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL},
      },
      std::vector<VkAttachmentReference>{
          {6, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL},
      },
      std::nullopt, std::vector<VkAttachmentReference>{},
      std::vector<uint32_t>{0, 1, 2, 4, 5});

  dependencies.emplace_back(VkSubpassDependency{
      VK_SUBPASS_EXTERNAL, 0, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_MEMORY_READ_BIT,
      VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
          VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      0});

  dependencies.push_back(VkSubpassDependency{
      0, 1, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
      VK_DEPENDENCY_BY_REGION_BIT});

  dependencies.push_back(VkSubpassDependency{
      1, 2, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
      VK_DEPENDENCY_BY_REGION_BIT});

  core_->Device()->CreateRenderPass(descriptions, subpasses, dependencies,
                                    &render_pass_);
}

void Renderer::DestroyRenderPass() {
  render_pass_.reset();
}

void Renderer::CreateEnvmapPipeline() {
  VkSampler sampler = sampler_->Handle();
  core_->Device()->CreateDescriptorSetLayout(
      {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
        VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_RAYGEN_BIT_KHR, nullptr},
       {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,
        VK_SHADER_STAGE_FRAGMENT_BIT, &sampler},
       {2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        nullptr}},
      &envmap_descriptor_set_layout_);
  core_->Device()->CreatePipelineLayout(
      {scene_descriptor_set_layout_->Handle(),
       envmap_descriptor_set_layout_->Handle()},
      &envmap_pipeline_layout_);

  core_->Device()->CreateShaderModule(
      vulkan::CompileGLSLToSPIRV(GetShaderCode("shaders/envmap_pass.vert"),
                                 VK_SHADER_STAGE_VERTEX_BIT),
      &envmap_vertex_shader_);
  core_->Device()->CreateShaderModule(
      vulkan::CompileGLSLToSPIRV(GetShaderCode("shaders/envmap_pass.frag"),
                                 VK_SHADER_STAGE_FRAGMENT_BIT),
      &envmap_fragment_shader_);

  vulkan::PipelineSettings pipeline_settings{render_pass_.get(),
                                             envmap_pipeline_layout_.get(), 0};
  pipeline_settings.depth_stencil_state_create_info->depthTestEnable = false;
  pipeline_settings.depth_stencil_state_create_info->depthWriteEnable = false;
  pipeline_settings.AddShaderStage(envmap_vertex_shader_.get(),
                                   VK_SHADER_STAGE_VERTEX_BIT);
  pipeline_settings.AddShaderStage(envmap_fragment_shader_.get(),
                                   VK_SHADER_STAGE_FRAGMENT_BIT);
  core_->Device()->CreatePipeline(pipeline_settings, &envmap_pipeline_);
}

void Renderer::DestroyEnvmapPipeline() {
  envmap_pipeline_.reset();

  envmap_vertex_shader_.reset();
  envmap_fragment_shader_.reset();

  envmap_pipeline_layout_.reset();
  envmap_descriptor_set_layout_.reset();
}

void Renderer::CreateEntityPipeline() {
  VkSampler sampler = sampler_->Handle();
  core_->Device()->CreateDescriptorSetLayout(
      {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, nullptr},
       {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, nullptr},
       {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,
        VK_SHADER_STAGE_FRAGMENT_BIT, &sampler},
       {3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,
        VK_SHADER_STAGE_FRAGMENT_BIT, &sampler}},
      &entity_descriptor_set_layout_);

  core_->Device()->CreatePipelineLayout(
      {scene_descriptor_set_layout_->Handle(),
       entity_descriptor_set_layout_->Handle()},
      &entity_pipeline_layout_);

  core_->Device()->CreateShaderModule(
      vulkan::CompileGLSLToSPIRV(GetShaderCode("shaders/entity_pass.vert"),
                                 VK_SHADER_STAGE_VERTEX_BIT),
      &entity_vertex_shader_);
  core_->Device()->CreateShaderModule(
      vulkan::CompileGLSLToSPIRV(GetShaderCode("shaders/entity_pass.frag"),
                                 VK_SHADER_STAGE_FRAGMENT_BIT),
      &entity_fragment_shader_);

  vulkan::PipelineSettings pipeline_settings{render_pass_.get(),
                                             entity_pipeline_layout_.get(), 0};
  pipeline_settings.AddInputBinding(0, sizeof(Vertex),
                                    VK_VERTEX_INPUT_RATE_VERTEX);
  pipeline_settings.AddInputAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT,
                                      offsetof(Vertex, position));
  pipeline_settings.AddInputAttribute(0, 1, VK_FORMAT_R32G32B32_SFLOAT,
                                      offsetof(Vertex, normal));
  pipeline_settings.AddInputAttribute(0, 2, VK_FORMAT_R32G32B32_SFLOAT,
                                      offsetof(Vertex, tangent));
  pipeline_settings.AddInputAttribute(0, 3, VK_FORMAT_R32G32_SFLOAT,
                                      offsetof(Vertex, tex_coord));
  pipeline_settings.AddInputAttribute(0, 4, VK_FORMAT_R32_SFLOAT,
                                      offsetof(Vertex, signal));

  pipeline_settings.AddShaderStage(entity_vertex_shader_.get(),
                                   VK_SHADER_STAGE_VERTEX_BIT);
  pipeline_settings.AddShaderStage(entity_fragment_shader_.get(),
                                   VK_SHADER_STAGE_FRAGMENT_BIT);

  pipeline_settings.SetPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
  pipeline_settings.SetCullMode(VK_CULL_MODE_NONE);

  core_->Device()->CreatePipeline(pipeline_settings, &entity_pipeline_);
}

void Renderer::DestroyEntityPipeline() {
  entity_pipeline_.reset();

  entity_vertex_shader_.reset();
  entity_fragment_shader_.reset();

  entity_pipeline_layout_.reset();
  entity_descriptor_set_layout_.reset();
}

void Renderer::CreateLightingPipeline() {
  Core()->Device()->CreateDescriptorSetLayout(
      {{0, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT,
        nullptr},
       {1, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT,
        nullptr},
       {2, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT,
        nullptr}},
      &lighting_descriptor_set_layout_);

  Core()->Device()->CreatePipelineLayout(
      {scene_descriptor_set_layout_->Handle(),
       lighting_descriptor_set_layout_->Handle()},
      &lighting_pipeline_layout_);
}

void Renderer::DestroyLightingPipeline() {
  lighting_pipeline_layout_.reset();
  lighting_descriptor_set_layout_.reset();
}

void Renderer::CreatePostProcessPipeline() {
  Core()->Device()->CreateDescriptorSetLayout(
      {{0, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT,
        nullptr}},
      &post_process_descriptor_set_layout_);
  Core()->Device()->CreatePipelineLayout(
      {scene_descriptor_set_layout_->Handle(),
       post_process_descriptor_set_layout_->Handle()},
      &post_process_pipeline_layout_);

  Core()->Device()->CreateShaderModule(
      vulkan::CompileGLSLToSPIRV(GetShaderCode("shaders/post_process.vert"),
                                 VK_SHADER_STAGE_VERTEX_BIT),
      &post_process_vertex_shader_);

  Core()->Device()->CreateShaderModule(
      vulkan::CompileGLSLToSPIRV(GetShaderCode("shaders/post_process.frag"),
                                 VK_SHADER_STAGE_FRAGMENT_BIT),
      &post_process_fragment_shader_);

  vulkan::PipelineSettings pipeline_settings{
      render_pass_.get(), post_process_pipeline_layout_.get(), 2};

  pipeline_settings.AddShaderStage(post_process_vertex_shader_.get(),
                                   VK_SHADER_STAGE_VERTEX_BIT);
  pipeline_settings.AddShaderStage(post_process_fragment_shader_.get(),
                                   VK_SHADER_STAGE_FRAGMENT_BIT);

  pipeline_settings.SetPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
  pipeline_settings.SetCullMode(VK_CULL_MODE_NONE);

  Core()->Device()->CreatePipeline(pipeline_settings, &post_process_pipeline_);
}

void Renderer::DestroyPostProcessPipeline() {
  post_process_pipeline_.reset();
  post_process_fragment_shader_.reset();
  post_process_vertex_shader_.reset();
  post_process_pipeline_layout_.reset();
  post_process_descriptor_set_layout_.reset();
}

void Renderer::CreateRayTracingPipeline() {
  core_->Device()->CreateDescriptorSetLayout(
      {{0, VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, 1,
        VK_SHADER_STAGE_RAYGEN_BIT_KHR, nullptr}},
      &raytracing_descriptor_set_layout_);

  core_->Device()->CreateDescriptorSetLayout(
      {{0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        nullptr},
       {1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        nullptr},
       {2, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        nullptr},
       {3, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        nullptr},
       {4, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        nullptr}},
      &raytracing_film_descriptor_set_layout_);

  core_->Device()->CreatePipelineLayout(
      {scene_descriptor_set_layout_->Handle(),
       raytracing_descriptor_set_layout_->Handle(),
       asset_manager_->DescriptorSetLayout()->Handle(),
       envmap_descriptor_set_layout_->Handle(),
       raytracing_film_descriptor_set_layout_->Handle()},
      &raytracing_pipeline_layout_);

  core_->Device()->CreateShaderModule(
      vulkan::CompileGLSLToSPIRV(GetShaderCode("shaders/raytracing.rgen"),
                                 VK_SHADER_STAGE_RAYGEN_BIT_KHR),
      &raytracing_raygen_shader_);

  core_->Device()->CreateShaderModule(
      vulkan::CompileGLSLToSPIRV(GetShaderCode("shaders/raytracing.rmiss"),
                                 VK_SHADER_STAGE_MISS_BIT_KHR),
      &raytracing_miss_shader_);

  core_->Device()->CreateShaderModule(
      vulkan::CompileGLSLToSPIRV(GetShaderCode("shaders/raytracing.rchit"),
                                 VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR),
      &raytracing_closest_hit_shader_);

  core_->Device()->CreateRayTracingPipeline(
      raytracing_pipeline_layout_.get(), raytracing_raygen_shader_.get(),
      raytracing_miss_shader_.get(), raytracing_closest_hit_shader_.get(),
      &raytracing_pipeline_);

  core_->Device()->CreateShaderBindingTable(raytracing_pipeline_.get(),
                                            &raytracing_sbt_);
}

void Renderer::DestroyRayTracingPipeline() {
  raytracing_sbt_.reset();
  raytracing_pipeline_.reset();
  raytracing_closest_hit_shader_.reset();
  raytracing_miss_shader_.reset();
  raytracing_raygen_shader_.reset();
  raytracing_pipeline_layout_.reset();
  raytracing_film_descriptor_set_layout_.reset();
  raytracing_descriptor_set_layout_.reset();
}

int Renderer::CreateScene(int max_entities, double_ptr<class Scene> pp_scene) {
  pp_scene.construct(this, max_entities);
  return 0;
}

int Renderer::CreateFilm(uint32_t width,
                         uint32_t height,
                         double_ptr<Film> pp_film) {
  Film film;
  film.renderer = this;
  Core()->Device()->CreateImage(kAlbedoFormat, VkExtent2D{width, height},
                                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                    VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                                &film.albedo_image);
  Core()->Device()->CreateImage(kPositionFormat, VkExtent2D{width, height},
                                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                    VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                                &film.position_image);
  Core()->Device()->CreateImage(kNormalFormat, VkExtent2D{width, height},
                                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                    VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                                &film.normal_image);
  Core()->Device()->CreateImage(kRadianceFormat, VkExtent2D{width, height},
                                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                    VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                                &film.radiance_image);
  Core()->Device()->CreateImage(kDepthFormat, VkExtent2D{width, height},
                                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                                &film.depth_image);
  Core()->Device()->CreateImage(kStencilFormat, VkExtent2D{width, height},
                                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                    VK_IMAGE_USAGE_STORAGE_BIT,
                                &film.stencil_image);
  Core()->Device()->CreateImage(
      kResultFormat, VkExtent2D{width, height},
      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
      &film.result_image);

  Core()->SingleTimeCommands([&](VkCommandBuffer cmd_buffer) {
    vulkan::TransitImageLayout(
        cmd_buffer, film.stencil_image->Handle(), VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT, 0, VK_ACCESS_TRANSFER_READ_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT);
  });

  RenderPass()->CreateFramebuffer(
      {film.albedo_image->ImageView(), film.position_image->ImageView(),
       film.normal_image->ImageView(), film.radiance_image->ImageView(),
       film.depth_image->ImageView(), film.stencil_image->ImageView(),
       film.result_image->ImageView()},
      VkExtent2D{width, height}, &film.framebuffer);

  Core()->Device()->CreateDescriptorPool(
      LightingDescriptorSetLayout()->GetPoolSize() +
          PostProcessDescriptorSetLayout()->GetPoolSize(),
      2, &film.descriptor_pool);

  film.descriptor_pool->AllocateDescriptorSet(
      LightingDescriptorSetLayout()->Handle(), &film.lighting_attachment_set);
  film.lighting_attachment_set->BindInputAttachment(0, film.albedo_image.get());
  film.lighting_attachment_set->BindInputAttachment(1,
                                                    film.position_image.get());
  film.lighting_attachment_set->BindInputAttachment(2, film.normal_image.get());

  film.descriptor_pool->AllocateDescriptorSet(
      PostProcessDescriptorSetLayout()->Handle(),
      &film.post_process_attachment_set);
  film.post_process_attachment_set->BindInputAttachment(
      0, film.radiance_image.get());

  pp_film.construct(std::move(film));
  return 0;
}

int Renderer::CreateRayTracingFilm(uint32_t width,
                                   uint32_t height,
                                   double_ptr<RayTracingFilm> pp_film) {
  RayTracingFilm film;

  film.renderer = this;

  Core()->Device()->CreateImage(
      VK_FORMAT_R32G32B32A32_SFLOAT, VkExtent2D{width, height},
      VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
          VK_IMAGE_USAGE_TRANSFER_DST_BIT,
      &film.accumulated_radiance_image);

  Core()->Device()->CreateImage(
      VK_FORMAT_R32G32B32A32_SFLOAT, VkExtent2D{width, height},
                                VK_IMAGE_USAGE_STORAGE_BIT |
                                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                    VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                                &film.accumulated_weight_image);
  Core()->Device()->CreateImage(
      VK_FORMAT_R32G32B32A32_SFLOAT, VkExtent2D{width, height},
                                VK_IMAGE_USAGE_STORAGE_BIT |
                                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                    VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                                &film.accumulated_phase_gb_image);

  Core()->Device()->CreateImage(
      VK_FORMAT_R32G32B32A32_SFLOAT, VkExtent2D{width, height},
      VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
      &film.raw_result_image);

  Core()->Device()->CreateImage(
      VK_FORMAT_R8G8B8A8_UNORM, VkExtent2D{width, height},
      VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
      &film.result_image);
  Core()->Device()->CreateDescriptorPool(
      raytracing_film_descriptor_set_layout_->GetPoolSize(), 1,
      &film.descriptor_pool);
  film.descriptor_pool->AllocateDescriptorSet(
      raytracing_film_descriptor_set_layout_->Handle(), &film.descriptor_set);
  film.descriptor_set->BindStorageImage(0, film.result_image.get());
  film.descriptor_set->BindStorageImage(1,
                                        film.accumulated_radiance_image.get());
  film.descriptor_set->BindStorageImage(2, film.accumulated_weight_image.get());
  film.descriptor_set->BindStorageImage(3, film.accumulated_phase_gb_image.get());
  film.descriptor_set->BindStorageImage(4, film.raw_result_image.get());

  Core()->Device()->NameObject(film.descriptor_set->Handle(),
                               "Ray Tracing Film Descriptor Set");

  Core()->SingleTimeCommands([&](VkCommandBuffer cmd_buffer) {
    vulkan::TransitImageLayout(
        cmd_buffer, film.result_image->Handle(), VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_GENERAL, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR, VK_ACCESS_MEMORY_READ_BIT,
        VK_ACCESS_SHADER_WRITE_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
    vulkan::TransitImageLayout(
        cmd_buffer, film.accumulated_radiance_image->Handle(),
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR, VK_ACCESS_MEMORY_READ_BIT,
        VK_ACCESS_SHADER_WRITE_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
    vulkan::TransitImageLayout(
        cmd_buffer, film.accumulated_weight_image->Handle(),
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR, VK_ACCESS_MEMORY_READ_BIT,
        VK_ACCESS_SHADER_WRITE_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
    vulkan::TransitImageLayout(
        cmd_buffer, film.accumulated_phase_gb_image->Handle(),
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR, VK_ACCESS_MEMORY_READ_BIT,
        VK_ACCESS_SHADER_WRITE_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
    vulkan::TransitImageLayout(
        cmd_buffer, film.raw_result_image->Handle(), VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_GENERAL, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR, VK_ACCESS_MEMORY_READ_BIT,
        VK_ACCESS_SHADER_WRITE_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
  });

  pp_film.construct(std::move(film));
  return 0;
}

void Renderer::RenderScene(VkCommandBuffer cmd_buffer,
                           Film *film,
                           Scene *scene) {
  std::vector<VkClearValue> clear_values(6);
  clear_values[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
  clear_values[1].color = {0.0f, 0.0f, 0.0f, 1.0f};
  clear_values[2].color = {0.0f, 0.0f, 0.0f, 1.0f};
  clear_values[3].color = {0.6f, 0.7f, 0.8f, 1.0f};
  clear_values[4].depthStencil = {1.0f, 0};
  clear_values[5].color = {-1, -1, -1, -1};
  VkRenderPassBeginInfo render_pass_begin_info{
      VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      nullptr,
      render_pass_->Handle(),
      film->framebuffer->Handle(),
      {{0, 0}, film->framebuffer->Extent()},
      static_cast<uint32_t>(clear_values.size()),
      clear_values.data()};
  vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info,
                       VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    envmap_pipeline_->Handle());

  // scissor and viewport
  VkViewport viewport = {0.0f,
                         0.0f,
                         static_cast<float>(film->framebuffer->Extent().width),
                         static_cast<float>(film->framebuffer->Extent().height),
                         0.0f,
                         1.0f};
  VkRect2D scissor = {{0, 0}, film->framebuffer->Extent()};
  vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
  vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);

  // bind descriptor set
  VkDescriptorSet descriptor_sets[] = {
      scene->SceneSettingsDescriptorSet(core_->CurrentFrame())};
  VkDescriptorSet far_descriptor_sets[] = {
      scene->FarSceneSettingsDescriptorSet(core_->CurrentFrame())};

  vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          entity_pipeline_layout_->Handle(), 0, 1,
                          far_descriptor_sets, 0, nullptr);

  scene->DrawEnvmap(cmd_buffer, core_->CurrentFrame());

  vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    entity_pipeline_->Handle());

  scene->DrawEntities(cmd_buffer, core_->CurrentFrame());

  VkClearAttachment clearAttachment = {};
  VkClearRect clearRect = {};

  clearAttachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  clearAttachment.clearValue.depthStencil = {1.0f, 0};

  // Define the area to clear (entire framebuffer in this example)
  clearRect.rect.offset = {0, 0};
  clearRect.rect.extent = film->framebuffer->Extent();
  clearRect.baseArrayLayer = 0;
  clearRect.layerCount = 1;

  // Ensure you are inside an active render pass
  vkCmdClearAttachments(cmd_buffer, 1, &clearAttachment, 1, &clearRect);

  vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          entity_pipeline_layout_->Handle(), 0, 1,
                          descriptor_sets, 0, nullptr);

  scene->DrawEntities(cmd_buffer, core_->CurrentFrame());

  vkCmdNextSubpass(cmd_buffer, VK_SUBPASS_CONTENTS_INLINE);

  vkCmdNextSubpass(cmd_buffer, VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    post_process_pipeline_->Handle());
  VkDescriptorSet film_post_process_descriptor_sets[] = {
      film->post_process_attachment_set->Handle()};
  vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          post_process_pipeline_layout_->Handle(), 1, 1,
                          film_post_process_descriptor_sets, 0, nullptr);
  vkCmdDraw(cmd_buffer, 6, 1, 0, 0);

  vkCmdEndRenderPass(cmd_buffer);
}

void Renderer::RenderSceneRayTracing(VkCommandBuffer cmd_buffer,
                                     RayTracingFilm *film,
                                     Scene *scene) {
  vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR,
                    raytracing_pipeline_->Handle());

  std::vector<VkDescriptorSet> descriptor_sets{
      scene->SceneSettingsDescriptorSet(core_->CurrentFrame()),
      scene->RayTracingDescriptorSet(core_->CurrentFrame()),
      asset_manager_->DescriptorSet(core_->CurrentFrame()),
      scene->GetEnvMap()->DescriptorSet(core_->CurrentFrame()),
      film->descriptor_set->Handle()};

  vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR,
                          raytracing_pipeline_layout_->Handle(), 0,
                          descriptor_sets.size(), descriptor_sets.data(), 0,
                          nullptr);

  auto aligned_size = [](uint32_t value, uint32_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
  };

  VkPhysicalDeviceRayTracingPipelinePropertiesKHR
      ray_tracing_pipeline_properties =
          core_->Device()
              ->PhysicalDevice()
              .GetPhysicalDeviceRayTracingPipelineProperties();

  const uint32_t handle_size_aligned =
      aligned_size(ray_tracing_pipeline_properties.shaderGroupHandleSize,
                   ray_tracing_pipeline_properties.shaderGroupHandleAlignment);

  VkStridedDeviceAddressRegionKHR ray_gen_shader_sbt_entry{};
  ray_gen_shader_sbt_entry.deviceAddress =
      raytracing_sbt_->GetRayGenDeviceAddress();
  ray_gen_shader_sbt_entry.stride = handle_size_aligned;
  ray_gen_shader_sbt_entry.size = handle_size_aligned;

  VkStridedDeviceAddressRegionKHR miss_shader_sbt_entry{};
  miss_shader_sbt_entry.deviceAddress = raytracing_sbt_->GetMissDeviceAddress();
  miss_shader_sbt_entry.stride = handle_size_aligned;
  miss_shader_sbt_entry.size = handle_size_aligned;

  VkStridedDeviceAddressRegionKHR hit_shader_sbt_entry{};
  hit_shader_sbt_entry.deviceAddress =
      raytracing_sbt_->GetClosestHitDeviceAddress();
  hit_shader_sbt_entry.stride = handle_size_aligned;
  hit_shader_sbt_entry.size = handle_size_aligned;

  VkStridedDeviceAddressRegionKHR callable_shader_sbt_entry{};
  core_->Device()->Procedures().vkCmdTraceRaysKHR(
      cmd_buffer, &ray_gen_shader_sbt_entry, &miss_shader_sbt_entry,
      &hit_shader_sbt_entry, &callable_shader_sbt_entry,
      film->result_image->Extent().width, film->result_image->Extent().height,
      1);

  SceneSettings settings;
  scene->GetSceneSettings(settings);
  settings.accumulated_sample += settings.num_sample;
  scene->SetSceneSettings(settings);
}
}  // namespace sparkium
