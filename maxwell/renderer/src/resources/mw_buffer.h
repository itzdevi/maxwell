#ifndef MW_BUFFER_H
#define MW_BUFFER_H

#include <mw_gpu.h>
#include <vulkan/vulkan.h>

#include "mw_renderer_vertex.h"

typedef struct mw_buffer {
  VkBuffer buffer;
  VkDeviceMemory memory;
  size_t size;
  size_t stride;
} mw_buffer;

mw_buffer mw_renderer_create_buffer(mw_device device,
                                    VkPhysicalDevice physicalDevice,
                                    VkSurfaceKHR surface, uint32_t size,
                                    uint32_t stride, VkBufferUsageFlags usage,
                                    VkMemoryPropertyFlags properties);
void mw_renderer_destroy_buffer(mw_device device, mw_buffer buffer);
void mw_renderer_copy_buffer(mw_device device, mw_buffer srcBuffer,
                             mw_buffer dstBuffer, mw_command transferCommand);

#endif