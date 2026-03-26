#ifndef MW_COMMAND_BUFFER_H
#define MW_COMMAND_BUFFER_H

#include <vulkan/vulkan.h>

#include "core/mw_device.h"

typedef enum mw_queue_family_type { GRAPHICS, TRANSFER } mw_queue_family_type;

typedef struct mw_command {
  VkCommandBuffer* buffers;
  VkCommandPool pool;
} mw_command;

mw_command mw_renderer_create_command(mw_device device,
                                      VkPhysicalDevice physicalDevice,
                                      VkSurfaceKHR surface,
                                      const unsigned int bufferCount,
                                      mw_queue_family_type family);
void mw_renderer_destroy_command(mw_device device, mw_command buffer);

#endif