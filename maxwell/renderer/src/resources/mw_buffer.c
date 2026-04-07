#include "mw_buffer.h"

#include <mw_core.h>

#include "core/mw_renderer_def.h"

uint32_t mw_renderer_find_memory_type(VkPhysicalDevice device,
                                      uint32_t typeFilter,
                                      VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memoryProperties;
  vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

  for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) &&
        (memoryProperties.memoryTypes[i].propertyFlags & properties) ==
            properties) {
      return i;
    }
  }

  MW_ASSERT(0);  // FAILED TO FIND SUITABLE MEMORY TYPE
}

mw_buffer mw_renderer_create_buffer(mw_renderer* renderer, uint32_t count,
                                    uint32_t stride, VkBufferUsageFlags usage,
                                    VkMemoryPropertyFlags properties) {
  VkBufferCreateInfo bufferInfo = {0};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = count * stride;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VkBuffer buffer;
  MW_ASSERT(vkCreateBuffer(renderer->gpu.device.device, &bufferInfo, NULL,
                           &buffer) == VK_SUCCESS);

  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(renderer->gpu.device.device, buffer,
                                &memoryRequirements);

  VkMemoryAllocateInfo allocInfo = {0};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memoryRequirements.size;
  allocInfo.memoryTypeIndex = mw_renderer_find_memory_type(
      renderer->gpu.physicalDevice, memoryRequirements.memoryTypeBits,
      properties);

  VkDeviceMemory memory;
  MW_ASSERT(vkAllocateMemory(renderer->gpu.device.device, &allocInfo, NULL,
                             &memory) == VK_SUCCESS);

  vkBindBufferMemory(renderer->gpu.device.device, buffer, memory, 0);

  return (mw_buffer){
      .buffer = buffer, .memory = memory, .size = count, .stride = stride};
}

void mw_renderer_destroy_buffer(mw_renderer* renderer, mw_buffer buffer) {
  vkDestroyBuffer(renderer->gpu.device.device, buffer.buffer, NULL);
  vkFreeMemory(renderer->gpu.device.device, buffer.memory, NULL);
}

void mw_renderer_copy_buffer(mw_renderer* renderer, mw_buffer srcBuffer,
                             mw_buffer dstBuffer) {
  VkCommandBufferAllocateInfo allocInfo = {0};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = renderer->transfer_command.pool;
  allocInfo.commandBufferCount = 1;

  VkCommandBufferBeginInfo beginInfo = {0};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(renderer->transfer_command.buffers[0], &beginInfo);

  VkBufferCopy copyRegion = {0};
  copyRegion.srcOffset = 0;
  copyRegion.dstOffset = 0;
  copyRegion.size = dstBuffer.size;
  vkCmdCopyBuffer(renderer->transfer_command.buffers[0], srcBuffer.buffer,
                  dstBuffer.buffer, 1, &copyRegion);

  vkEndCommandBuffer(renderer->transfer_command.buffers[0]);

  VkSubmitInfo submitInfo = {0};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &renderer->transfer_command.buffers[0];

  vkQueueSubmit(renderer->gpu.device.transferQueue, 1, &submitInfo,
                VK_NULL_HANDLE);
  vkQueueWaitIdle(renderer->gpu.device.transferQueue);
}