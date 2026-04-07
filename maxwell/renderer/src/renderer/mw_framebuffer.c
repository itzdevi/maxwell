#include "mw_framebuffer.h"

#include <mw_core.h>

#include "core/mw_renderer_def.h"

void mw_renderer_create_framebuffers(mw_renderer* renderer) {
  VkFramebuffer* framebuffers =
      malloc(renderer->swapchain.image_count * sizeof(VkFramebuffer));

  for (size_t i = 0; i < renderer->swapchain.image_count; i++) {
    VkImageView attachments[] = {renderer->swapchain.image_views[i],
                                 renderer->swapchain.depth_image_view};

    VkFramebufferCreateInfo framebufferCreateInfo = {0};
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass = renderer->pipeline.render_pass;
    framebufferCreateInfo.attachmentCount = 2;
    framebufferCreateInfo.pAttachments = attachments;
    framebufferCreateInfo.width = renderer->swapchain.extent.width;
    framebufferCreateInfo.height = renderer->swapchain.extent.height;
    framebufferCreateInfo.layers = 1;

    MW_ASSERT(vkCreateFramebuffer(renderer->gpu.device.device,
                                  &framebufferCreateInfo, NULL,
                                  &framebuffers[i]) == VK_SUCCESS);
  }

  renderer->framebuffers = framebuffers;
}

void mw_renderer_destroy_framebuffers(mw_device device, mw_swapchain swapchain,
                                      VkFramebuffer* framebuffers) {
  for (size_t i = 0; i < swapchain.image_count; i++)
    vkDestroyFramebuffer(device.device, framebuffers[i], NULL);

  free(framebuffers);
}