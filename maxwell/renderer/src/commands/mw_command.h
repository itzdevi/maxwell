#ifndef MW_COMMAND_BUFFER_H
#define MW_COMMAND_BUFFER_H

#include <vulkan/vulkan.h>

typedef struct mw_command_buffers {
  VkCommandBuffer* buffers;
  VkCommandPool pool;
} mw_command_buffers;

#endif