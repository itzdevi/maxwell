#include "mw_vertex_buffer.h"

#include <stdio.h>
#include <string.h>

#include "core/mw_renderer_def.h"
#include "mw_renderer_vertex.h"

mw_buffer mw_renderer_create_vertex_buffer(mw_renderer* renderer,
                                           mw_vertex* vertices,
                                           uint32_t verticesCount,
                                           void* pData) {
  VkDeviceSize bufferSize = verticesCount * sizeof(mw_vertex);
  mw_buffer stagingBuffer = mw_renderer_create_buffer(
      renderer, bufferSize / sizeof(mw_vertex), sizeof(mw_vertex),
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  vkMapMemory(renderer->gpu.device.device, stagingBuffer.memory, 0, bufferSize,
              0, &pData);
  memcpy(pData, vertices, bufferSize);
  vkUnmapMemory(renderer->gpu.device.device, stagingBuffer.memory);

  mw_buffer vertexBuffer = mw_renderer_create_buffer(
      renderer, bufferSize, sizeof(mw_vertex),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  mw_renderer_copy_buffer(renderer, stagingBuffer, vertexBuffer);
  mw_renderer_destroy_buffer(renderer, stagingBuffer);

  return vertexBuffer;
}