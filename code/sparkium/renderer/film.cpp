#include "sparkium/renderer/film.h"

#include "sparkium/renderer/renderer.h"

namespace sparkium {

void Film::Resize(uint32_t width, uint32_t height) {
  VkExtent2D extent{width, height};
  albedo_image->Resize(extent);
  position_image->Resize(extent);
  normal_image->Resize(extent);
  radiance_image->Resize(extent);
  depth_image->Resize(extent);
  stencil_image->Resize(extent);
  result_image->Resize(extent);
  renderer->RenderPass()->CreateFramebuffer(
      {albedo_image->ImageView(), position_image->ImageView(),
       normal_image->ImageView(), radiance_image->ImageView(),
       depth_image->ImageView(), stencil_image->ImageView(),
       result_image->ImageView()},
      VkExtent2D{width, height}, &framebuffer);

  lighting_attachment_set->BindInputAttachment(0, albedo_image.get());
  lighting_attachment_set->BindInputAttachment(1, position_image.get());
  lighting_attachment_set->BindInputAttachment(2, normal_image.get());
  post_process_attachment_set->BindInputAttachment(0, radiance_image.get());

  renderer->Core()->SingleTimeCommands(
      [image = stencil_image->Handle()](VkCommandBuffer cmd_buffer) {
        vulkan::TransitImageLayout(
            cmd_buffer, image, VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            0, VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
      });
}

}  // namespace sparkium
