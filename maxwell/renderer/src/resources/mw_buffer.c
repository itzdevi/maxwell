#include "mw_buffer.h"

#include "core/mw_queue.h"
#include "mw_assert.h"

uint32_t find_memory_type(VkPhysicalDevice device, uint32_t typeFilter,
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

mw_buffer mw_renderer_create_buffer(mw_device device,
                                    VkPhysicalDevice physicalDevice,
                                    VkSurfaceKHR surface, uint32_t size,
                                    uint32_t stride, VkBufferUsageFlags usage,
                                    VkMemoryPropertyFlags properties) {
  VkBufferCreateInfo bufferInfo = {0};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VkBuffer buffer;
  MW_ASSERT(vkCreateBuffer(device.device, &bufferInfo, NULL, &buffer) ==
            VK_SUCCESS);

  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(device.device, buffer, &memoryRequirements);

  VkMemoryAllocateInfo allocInfo = {0};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memoryRequirements.size;
  allocInfo.memoryTypeIndex = find_memory_type(
      physicalDevice, memoryRequirements.memoryTypeBits, properties);

  VkDeviceMemory memory;
  MW_ASSERT(vkAllocateMemory(device.device, &allocInfo, NULL, &memory) ==
            VK_SUCCESS);

  vkBindBufferMemory(device.device, buffer, memory, 0);

  return (mw_buffer){
      .buffer = buffer, .memory = memory, .size = size, .stride = stride};
}

void mw_renderer_destroy_buffer(mw_device device, mw_buffer buffer) {
  vkDestroyBuffer(device.device, buffer.buffer, NULL);
  vkFreeMemory(device.device, buffer.memory, NULL);
}