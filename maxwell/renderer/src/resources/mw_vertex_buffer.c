#include "mw_vertex_buffer.h"

#include <stdio.h>
#include <string.h>

#include "mw_renderer_vertex.h"

mw_buffer mw_renderer_create_vertex_buffer(
    mw_device device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
    mw_vertex* vertices, uint32_t verticesCount, mw_command transferCommand,
    void* pData) {
  VkDeviceSize bufferSize = verticesCount * sizeof(mw_vertex);
  mw_buffer stagingBuffer = mw_renderer_create_buffer(
      device, physicalDevice, surface, bufferSize / sizeof(mw_vertex),
      sizeof(mw_vertex), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  vkMapMemory(device.device, stagingBuffer.memory, 0, bufferSize, 0, &pData);
  memcpy(pData, vertices, bufferSize);
  vkUnmapMemory(device.device, stagingBuffer.memory);

  mw_buffer vertexBuffer = mw_renderer_create_buffer(
      device, physicalDevice, surface, bufferSize, sizeof(mw_vertex),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  mw_renderer_copy_buffer(device, stagingBuffer, vertexBuffer, transferCommand);
  mw_renderer_destroy_buffer(device, stagingBuffer);

  return vertexBuffer;
}