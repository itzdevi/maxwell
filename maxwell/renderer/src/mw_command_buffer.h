#ifndef MW_COMMAND_BUFFER_H
#define MW_COMMAND_BUFFER_H

#include <vulkan/vulkan.h>

#include "mw_device.h"

typedef struct mw_command_buffer {
  VkCommandBuffer buffer;
  VkCommandPool pool;
} mw_command_buffer;

mw_command_buffer mw_renderer_create_command_buffer(
    mw_device device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
void mw_renderer_destroy_command_buffer(mw_device device,
                                        mw_command_buffer buffer);

#endif