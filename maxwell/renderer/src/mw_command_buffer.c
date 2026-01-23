#include "mw_command_buffer.h"

#include "mw_assert.h"
#include "mw_render_queue.h"

VkCommandPool mw_create_command_pool(VkDevice device,
                                     VkPhysicalDevice physicalDevice,
                                     VkSurfaceKHR surface) {
  mw_queue_family_indices indices =
      mw_renderer_find_queue_families(physicalDevice, surface);

  VkCommandPoolCreateInfo commandPoolCreateInfo = {0};
  commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  commandPoolCreateInfo.queueFamilyIndex = indices.graphicsFamily;

  VkCommandPool pool;
  MW_ASSERT(vkCreateCommandPool(device, &commandPoolCreateInfo, NULL, &pool) ==
            VK_SUCCESS);
  return pool;
}

mw_command_buffer mw_renderer_create_command_buffer(
    mw_device device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
  mw_command_buffer buffer;
  buffer.pool = mw_create_command_pool(device.device, physicalDevice, surface);

  VkCommandBufferAllocateInfo commandBufferAllocInfo = {0};
  commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocInfo.commandBufferCount = 1;
  commandBufferAllocInfo.commandPool = buffer.pool;
  commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

  MW_ASSERT(vkAllocateCommandBuffers(device.device, &commandBufferAllocInfo,
                                     &buffer.buffer) == VK_SUCCESS);

  return buffer;
}

void mw_renderer_destroy_command_buffer(mw_device device,
                                        mw_command_buffer buffer) {
  vkDestroyCommandPool(device.device, buffer.pool, NULL);
}