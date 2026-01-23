#ifndef MW_COMMAND_BUFFER_H
#define MW_COMMAND_BUFFER_H

#include <vulkan/vulkan.h>

#include "mw_device.h"
#include "mw_pipeline.h"
#include "mw_swapchain.h"

typedef struct mw_command_buffer {
  VkCommandBuffer buffer;
  VkCommandPool pool;
} mw_command_buffer;

void mw_renderer_record_command_buffer(mw_swapchain swapchain,
                                       mw_pipeline pipeline,
                                       mw_command_buffer commandBuffer,
                                       VkFramebuffer* framebuffers,
                                       unsigned int imageIndex);

mw_command_buffer mw_renderer_create_command_buffer(
    mw_device device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
void mw_renderer_destroy_command_buffer(mw_device device,
                                        mw_command_buffer buffer);

#endif