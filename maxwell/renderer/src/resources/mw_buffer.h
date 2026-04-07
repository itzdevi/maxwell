#ifndef MW_BUFFER_H
#define MW_BUFFER_H

#include <mw_gpu.h>
#include <vulkan/vulkan.h>

#include "mw_renderer.h"

typedef struct mw_buffer {
  VkBuffer buffer;
  VkDeviceMemory memory;
  size_t size;
  size_t stride;
} mw_buffer;

uint32_t mw_renderer_find_memory_type(VkPhysicalDevice device,
                                      uint32_t typeFilter,
                                      VkMemoryPropertyFlags properties);
mw_buffer mw_renderer_create_buffer(mw_renderer* renderer, uint32_t count,
                                    uint32_t stride, VkBufferUsageFlags usage,
                                    VkMemoryPropertyFlags properties);
void mw_renderer_destroy_buffer(mw_renderer* renderer, mw_buffer buffer);
void mw_renderer_copy_buffer(mw_renderer* renderer, mw_buffer srcBuffer,
                             mw_buffer dstBuffer);

#endif