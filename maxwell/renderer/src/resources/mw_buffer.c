#include "mw_buffer.h"

#include <string.h>

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
                                    uint32_t size, uint32_t stride) {
  VkBufferCreateInfo bufferInfo = {0};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VkBuffer buffer;
  MW_ASSERT(vkCreateBuffer(device.device, &bufferInfo, NULL, &buffer) ==
            VK_SUCCESS);

  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(device.device, buffer, &memoryRequirements);

  VkMemoryAllocateInfo allocInfo = {0};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memoryRequirements.size;
  allocInfo.memoryTypeIndex =
      find_memory_type(physicalDevice, memoryRequirements.memoryTypeBits,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  VkDeviceMemory memory;
  MW_ASSERT(vkAllocateMemory(device.device, &allocInfo, NULL, &memory) ==
            VK_SUCCESS);

  vkBindBufferMemory(device.device, buffer, memory, 0);

  void* data;
  vkMapMemory(device.device, memory, 0, memoryRequirements.size, 0, &data);

  return (mw_buffer){.buffer = buffer,
                     .memory = memory,
                     .data = data,
                     .size = size,
                     .stride = stride};
}

void mw_renderer_copy_memory(mw_buffer buffer, void* data) {
  memcpy(buffer.data, data, buffer.size);
}

void mw_renderer_destroy_buffer(mw_device device, mw_buffer buffer) {
  vkUnmapMemory(device.device, buffer.memory);
  vkDestroyBuffer(device.device, buffer.buffer, NULL);
  vkFreeMemory(device.device, buffer.memory, NULL);
}