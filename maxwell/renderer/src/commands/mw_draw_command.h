#ifndef MW_DRAW_BUFFER_H
#define MW_DRAW_BUFFER_H

#include <vulkan/vulkan.h>

#include "core/mw_device.h"
#include "mw_command.h"
#include "renderer/mw_pipeline.h"
#include "renderer/mw_swapchain.h"
#include "resources/mw_buffer.h"

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