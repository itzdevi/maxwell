#include "mw_index_buffer.h"

#include <string.h>

#include "core/mw_renderer_def.h"

mw_buffer mw_renderer_create_index_buffer(mw_renderer* renderer,
                                          uint32_t* indices,
                                          uint32_t indicesCount, void* pData) {
  VkDeviceSize bufferSize = indicesCount * sizeof(uint32_t);
  mw_buffer stagingBuffer = mw_renderer_create_buffer(
      renderer, bufferSize / sizeof(uint32_t), sizeof(uint32_t),
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  vkMapMemory(renderer->gpu.device.device, stagingBuffer.memory, 0, bufferSize,
              0, &pData);
  memcpy(pData, indices, bufferSize);
  vkUnmapMemory(renderer->gpu.device.device, stagingBuffer.memory);

  mw_buffer indexBuffer = mw_renderer_create_buffer(
      renderer, indicesCount, sizeof(uint32_t),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  mw_renderer_copy_buffer(renderer, stagingBuffer, indexBuffer);
  mw_renderer_destroy_buffer(renderer, stagingBuffer);

  return indexBuffer;
}