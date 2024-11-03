#include "sparkium/app/app_gui_renderer.h"

#include "sparkium/app/app.h"
#include "vector"

namespace sparkium {

namespace {

#include "built_in_shaders.inl"

}

AppGuiRenderer::AppGuiRenderer(Application *app) : app_(app) {
  app_->Core()->CreateDynamicBuffer<AppGuiInfo>(
      1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &gui_info_buffer_);

  app_->Core()->Device()->CreateDescriptorSetLayout(
      {
          {0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1,
           VK_SHADER_STAGE_FRAGMENT_BIT},
          {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
           VK_SHADER_STAGE_FRAGMENT_BIT},
      },
      &descriptor_set_layout_);

  app_->Core()->Device()->CreateDescriptorPool(
      descriptor_set_layout_->GetPoolSize() * app_->Core()->MaxFramesInFlight(),
      app_->Core()->MaxFramesInFlight(), &descriptor_pool_);
  app_->Core()->Device()->CreatePipelineLayout(
      {descriptor_set_layout_->Handle()}, &pipeline_layout_);
  descriptor_sets_.resize(app_->Core()->MaxFramesInFlight());
  for (size_t frame_id = 0; frame_id < app_->Core()->MaxFramesInFlight();
       frame_id++) {
    auto &descriptor_set = descriptor_sets_[frame_id];
    descriptor_pool_->AllocateDescriptorSet(descriptor_set_layout_->Handle(),
                                            &descriptor_set);
    descriptor_set->BindUniformBuffer(1, gui_info_buffer_->GetBuffer(frame_id));
  }

  app_->Core()->Device()->CreateShaderModule(
      vulkan::CompileGLSLToSPIRV(GetShaderCode("shaders/app_gui.vert"),
                                 VK_SHADER_STAGE_VERTEX_BIT),
      &vertex_shader_);
  app_->Core()->Device()->CreateShaderModule(
      vulkan::CompileGLSLToSPIRV(GetShaderCode("shaders/app_gui.frag"),
                                 VK_SHADER_STAGE_FRAGMENT_BIT),
      &fragment_shader_);

  app_->Core()->Device()->CreateRenderPass(
      std::vector<VkAttachmentDescription>{
          {0, VK_FORMAT_B8G8R8A8_UNORM, VK_SAMPLE_COUNT_1_BIT,
           VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_STORE_OP_STORE,
           VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
           VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_GENERAL}},
      std::vector<VkAttachmentReference>{
          {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}},
      &render_pass_);

  vulkan::PipelineSettings pipeline_settings{render_pass_.get(),
                                             pipeline_layout_.get()};
  pipeline_settings.AddShaderStage(vertex_shader_.get(),
                                   VK_SHADER_STAGE_VERTEX_BIT);
  pipeline_settings.AddShaderStage(fragment_shader_.get(),
                                   VK_SHADER_STAGE_FRAGMENT_BIT);
  pipeline_settings.SetPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
  pipeline_settings.SetMultiSampleState(VK_SAMPLE_COUNT_1_BIT);
  pipeline_settings.SetCullMode(VK_CULL_MODE_NONE);
  pipeline_settings.SetBlendState(
      0, VkPipelineColorBlendAttachmentState{
             VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA,
             VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
             VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_FACTOR_ONE,
             VK_BLEND_OP_ADD,
             VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                 VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT});
  app_->Core()->Device()->CreatePipeline(pipeline_settings, &pipeline_);
}

AppGuiRenderer::~AppGuiRenderer() {
  pipeline_.reset();
  framebuffer_.reset();
  render_pass_.reset();
  vertex_shader_.reset();
  fragment_shader_.reset();

  descriptor_sets_.clear();
  pipeline_layout_.reset();
  descriptor_pool_.reset();
  descriptor_set_layout_.reset();
  gui_info_buffer_.reset();
}

void AppGuiRenderer::Render(VkCommandBuffer cmd_buffer) {
  VkRenderPassBeginInfo render_pass_begin_info{};
  render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_begin_info.renderPass = render_pass_->Handle();
  render_pass_begin_info.framebuffer = framebuffer_->Handle();
  render_pass_begin_info.renderArea.offset = {0, 0};
  render_pass_begin_info.renderArea.extent = framebuffer_->Extent();
  render_pass_begin_info.clearValueCount = 0;
  render_pass_begin_info.pClearValues = nullptr;

  vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipeline_->Handle());

  auto descriptor_set =
      descriptor_sets_[app_->Core()->CurrentFrame()]->Handle();
  vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipeline_layout_->Handle(), 0, 1, &descriptor_set, 0,
                          nullptr);
  vkCmdDraw(cmd_buffer, 6, 1, 0, 0);

  vkCmdEndRenderPass(cmd_buffer);
}

void AppGuiRenderer::UpdateGuiInfo(const AppGuiInfo &gui_info) {
  gui_info_buffer_->At(0) = gui_info;
}

void AppGuiRenderer::SyncData(VkCommandBuffer cmd_buffer, uint32_t frame_id) {
  gui_info_buffer_->SyncData(cmd_buffer, frame_id);
}

void AppGuiRenderer::BindRelatedImages(vulkan::Image *image,
                                       vulkan::Image *stencil_image) {
  framebuffer_.reset();
  render_pass_->CreateFramebuffer({image->ImageView()}, image->Extent(),
                                  &framebuffer_);
  for (uint32_t frame_id = 0; frame_id < app_->Core()->MaxFramesInFlight();
       frame_id++) {
    descriptor_sets_[frame_id]->BindStorageImage(0, stencil_image);
  }
}
}  // namespace sparkium
