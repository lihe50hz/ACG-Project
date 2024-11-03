#pragma once
#include "sparkium/app/app_utils.h"

namespace sparkium {

struct AppGuiInfo {
  int selected_instance_id[2];
  int hovering_instance_id[2];
};

class AppGuiRenderer {
 public:
  AppGuiRenderer(Application *app);
  ~AppGuiRenderer();

  void Render(VkCommandBuffer cmd_buffer);

  void UpdateGuiInfo(const AppGuiInfo &gui_info);

  void SyncData(VkCommandBuffer cmd_buffer, uint32_t frame_id);

  void BindRelatedImages(vulkan::Image *image, vulkan::Image *stencil_image);

 private:
  Application *app_;

  std::unique_ptr<vulkan::DynamicBuffer<AppGuiInfo>> gui_info_buffer_;

  std::unique_ptr<vulkan::DescriptorSetLayout> descriptor_set_layout_;
  std::unique_ptr<vulkan::DescriptorPool> descriptor_pool_;
  std::vector<std::unique_ptr<vulkan::DescriptorSet>> descriptor_sets_;
  std::unique_ptr<vulkan::PipelineLayout> pipeline_layout_;

  std::unique_ptr<vulkan::ShaderModule> vertex_shader_;
  std::unique_ptr<vulkan::ShaderModule> fragment_shader_;

  std::unique_ptr<vulkan::RenderPass> render_pass_;
  std::unique_ptr<vulkan::Framebuffer> framebuffer_;

  std::unique_ptr<vulkan::Pipeline> pipeline_;
};
}  // namespace sparkium
