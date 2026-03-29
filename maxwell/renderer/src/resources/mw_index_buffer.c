#include "mw_index_buffer.h"

#include <string.h>

mw_buffer mw_renderer_create_index_buffer(
    mw_device device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
    uint16_t* indices, uint32_t indicesCount, mw_command transferCommand,
    void* pData) {
  VkDeviceSize bufferSize = indicesCount * sizeof(uint16_t);
  mw_buffer stagingBuffer = mw_renderer_create_buffer(
      device, physicalDevice, surface, bufferSize / sizeof(uint16_t),
      sizeof(uint16_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  vkMapMemory(device.device, stagingBuffer.memory, 0, bufferSize, 0, &pData);
  memcpy(pData, indices, bufferSize);
  vkUnmapMemory(device.device, stagingBuffer.memory);

  mw_buffer indexBuffer = mw_renderer_create_buffer(
      device, physicalDevice, surface, bufferSize, sizeof(uint16_t),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  mw_renderer_copy_buffer(device, stagingBuffer, indexBuffer, transferCommand);
  mw_renderer_destroy_buffer(device, stagingBuffer);

  return indexBuffer;
}