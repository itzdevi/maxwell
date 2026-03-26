#include "mw_framebuffer.h"

#include "mw_assert.h"

VkFramebuffer* mw_renderer_create_framebuffers(mw_device device,
                                               mw_swapchain swapchain,
                                               mw_pipeline pipeline) {
  VkFramebuffer* framebuffers =
      malloc(swapchain.imageCount * sizeof(VkFramebuffer));

  for (size_t i = 0; i < swapchain.imageCount; i++) {
    VkImageView attachments[] = {swapchain.imageViews[i]};

    VkFramebufferCreateInfo framebufferCreateInfo = {0};
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass = pipeline.renderPass;
    framebufferCreateInfo.attachmentCount = 1;
    framebufferCreateInfo.pAttachments = attachments;
    framebufferCreateInfo.width = swapchain.extent.width;
    framebufferCreateInfo.height = swapchain.extent.height;
    framebufferCreateInfo.layers = 1;

    MW_ASSERT(vkCreateFramebuffer(device.device, &framebufferCreateInfo, NULL,
                                  &framebuffers[i]) == VK_SUCCESS);
  }

  return framebuffers;
}

void mw_renderer_destroy_framebuffers(mw_device device, mw_swapchain swapchain,
                                      VkFramebuffer* framebuffers) {
  for (size_t i = 0; i < swapchain.imageCount; i++)
    vkDestroyFramebuffer(device.device, framebuffers[i], NULL);

  free(framebuffers);
}