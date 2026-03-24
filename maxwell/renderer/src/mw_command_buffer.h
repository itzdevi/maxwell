#ifndef MW_COMMAND_BUFFER_H
#define MW_COMMAND_BUFFER_H

#include <vulkan/vulkan.h>

#include "mw_buffer.h"
#include "mw_device.h"
#include "mw_pipeline.h"
#include "mw_swapchain.h"

typedef struct mw_command_buffers {
  VkCommandBuffer* buffers;
  VkCommandPool pool;
} mw_command_buffers;

void mw_renderer_record_command_buffer(
    mw_swapchain swapchain, mw_pipeline pipeline,
    mw_command_buffers commandBuffer, VkFramebuffer* framebuffers,
    mw_buffer buffer, unsigned int imageIndex, unsigned int frameIndex);

mw_command_buffers mw_renderer_create_command_buffers(
    mw_device device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
    const unsigned int maxFramesInFlight);
void mw_renderer_destroy_command_buffers(mw_device device,
                                         mw_command_buffers buffer);

#endif