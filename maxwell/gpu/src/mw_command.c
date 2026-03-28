#include "mw_command.h"

#include "mw_core.h"
#include "mw_queue.h"

VkCommandPool create_command_pool(VkDevice device,
                                  VkPhysicalDevice physicalDevice,
                                  VkSurfaceKHR surface,
                                  mw_queue_family_type family) {
  mw_queue_family_indices indices =
      mw_gpu_find_queue_families(physicalDevice, surface);

  VkCommandPoolCreateInfo commandPoolCreateInfo = {0};
  commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

  switch (family) {
    case GRAPHICS:
      commandPoolCreateInfo.queueFamilyIndex = indices.graphicsFamily;
      break;
    case TRANSFER:
      commandPoolCreateInfo.queueFamilyIndex = indices.transferFamily;
      break;
    default:
      MW_ASSERT(0);  // UNDEFINED BEHAVIOR
      break;
  }

  VkCommandPool pool;
  MW_ASSERT(vkCreateCommandPool(device, &commandPoolCreateInfo, NULL, &pool) ==
            VK_SUCCESS);
  return pool;
}

mw_command mw_gpu_create_command(mw_device device,
                                 VkPhysicalDevice physicalDevice,
                                 VkSurfaceKHR surface,
                                 const uint16_t bufferCount,
                                 mw_queue_family_type family) {
  mw_command buffers;
  buffers.buffers = malloc(bufferCount * sizeof(VkCommandBuffer));
  buffers.pool =
      create_command_pool(device.device, physicalDevice, surface, family);
  buffers.count = bufferCount;

  VkCommandBufferAllocateInfo commandBufferAllocInfo = {0};
  commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocInfo.commandBufferCount = bufferCount;
  commandBufferAllocInfo.commandPool = buffers.pool;
  commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

  MW_ASSERT(vkAllocateCommandBuffers(device.device, &commandBufferAllocInfo,
                                     buffers.buffers) == VK_SUCCESS);

  return buffers;
}

void mw_gpu_destroy_command(mw_device device, mw_command buffers) {
  vkDestroyCommandPool(device.device, buffers.pool, NULL);

  free(buffers.buffers);
}
