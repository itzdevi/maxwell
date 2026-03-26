#include "mw_vertex_buffer.h"

#include <stdio.h>
#include <string.h>

#include "mw_renderer_vertex.h"

void copyBuffer(mw_device device, mw_buffer srcBuffer, mw_buffer dstBuffer,
                mw_command transferCommand) {
  VkCommandBufferAllocateInfo allocInfo = {0};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = transferCommand.pool;
  allocInfo.commandBufferCount = 1;

  VkCommandBufferBeginInfo beginInfo = {0};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(transferCommand.buffers[0], &beginInfo);

  VkBufferCopy copyRegion = {0};
  copyRegion.srcOffset = 0;
  copyRegion.dstOffset = 0;
  copyRegion.size = dstBuffer.size;
  vkCmdCopyBuffer(transferCommand.buffers[0], srcBuffer.buffer,
                  dstBuffer.buffer, 1, &copyRegion);

  vkEndCommandBuffer(transferCommand.buffers[0]);

  VkSubmitInfo submitInfo = {0};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &transferCommand.buffers[0];

  vkQueueSubmit(device.transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(device.transferQueue);

  vkFreeCommandBuffers(device.device, transferCommand.pool, 1,
                       &transferCommand.buffers[0]);
}

mw_buffer mw_renderer_create_vertex_buffer(
    mw_device device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
    mw_vertex* vertices, uint32_t verticesCount, mw_command transferCommand,
    void* pData) {
  VkDeviceSize bufferSize = verticesCount * sizeof(mw_vertex);
  mw_buffer stagingBuffer = mw_renderer_create_buffer(
      device, physicalDevice, surface, bufferSize, sizeof(mw_vertex),
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  vkMapMemory(device.device, stagingBuffer.memory, 0, bufferSize, 0, &pData);
  memcpy(pData, vertices, bufferSize);
  vkUnmapMemory(device.device, stagingBuffer.memory);

  mw_buffer vertexBuffer = mw_renderer_create_buffer(
      device, physicalDevice, surface, bufferSize, sizeof(mw_vertex),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  copyBuffer(device, stagingBuffer, vertexBuffer, transferCommand);
  mw_renderer_destroy_buffer(device, stagingBuffer);

  return vertexBuffer;
}