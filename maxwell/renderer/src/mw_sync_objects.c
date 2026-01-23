#include "mw_sync_objects.h"

#include "mw_assert.h"

mw_sync_objects mw_renderer_create_sync_objects(mw_device device) {
  mw_sync_objects syncObjects;

  VkSemaphoreCreateInfo semaphoreCreateInfo = {0};
  semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceCreateInfo = {0};
  fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  MW_ASSERT(
      vkCreateSemaphore(device.device, &semaphoreCreateInfo, NULL,
                        &syncObjects.imageAvailableSemaphore) == VK_SUCCESS &&
      vkCreateSemaphore(device.device, &semaphoreCreateInfo, NULL,
                        &syncObjects.renderFinishedSemaphore) == VK_SUCCESS &&
      vkCreateFence(device.device, &fenceCreateInfo, NULL,
                    &syncObjects.inFlightFence) == VK_SUCCESS);

  return syncObjects;
}

void mw_renderer_destroy_sync_objects(mw_device device,
                                      mw_sync_objects syncObjects) {
  vkDestroySemaphore(device.device, syncObjects.imageAvailableSemaphore, NULL);
  vkDestroySemaphore(device.device, syncObjects.renderFinishedSemaphore, NULL);
  vkDestroyFence(device.device, syncObjects.inFlightFence, NULL);
}