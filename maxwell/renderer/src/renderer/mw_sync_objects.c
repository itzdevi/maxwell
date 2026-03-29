#include "mw_sync_objects.h"

#include <mw_core.h>

mw_sync_objects mw_renderer_create_sync_objects(
    mw_device device, const unsigned int maxFramesInFlight,
    const unsigned int swapchainImageCount) {
  mw_sync_objects syncObjects;
  syncObjects.framesInFlight = maxFramesInFlight;
  syncObjects.swapchainImages = swapchainImageCount;

  syncObjects.acquireSemaphores =
      malloc(maxFramesInFlight * sizeof(VkSemaphore));
  syncObjects.submitSemaphores =
      malloc(swapchainImageCount * sizeof(VkSemaphore));
  syncObjects.frameFences = malloc(maxFramesInFlight * sizeof(VkFence));

  VkSemaphoreCreateInfo semaphoreCreateInfo = {0};
  semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceCreateInfo = {0};
  fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < maxFramesInFlight || i < swapchainImageCount; i++) {
    if (i < swapchainImageCount)
      MW_ASSERT(vkCreateSemaphore(device.device, &semaphoreCreateInfo, NULL,
                                  syncObjects.submitSemaphores + i) ==
                VK_SUCCESS);
    if (i < maxFramesInFlight)
      MW_ASSERT(vkCreateSemaphore(device.device, &semaphoreCreateInfo, NULL,
                                  syncObjects.acquireSemaphores + i) ==
                    VK_SUCCESS &&
                vkCreateFence(device.device, &fenceCreateInfo, NULL,
                              syncObjects.frameFences + i) == VK_SUCCESS);
  }

  return syncObjects;
}

void mw_renderer_destroy_sync_objects(mw_device device,
                                      mw_sync_objects syncObjects) {
  for (size_t i = 0;
       i < syncObjects.framesInFlight || i < syncObjects.swapchainImages; i++) {
    if (i < syncObjects.swapchainImages)
      vkDestroySemaphore(device.device, syncObjects.submitSemaphores[i], NULL);
    if (i < syncObjects.framesInFlight) {
      vkDestroySemaphore(device.device, syncObjects.acquireSemaphores[i], NULL);
      vkDestroyFence(device.device, syncObjects.frameFences[i], NULL);
    }
  }

  free(syncObjects.submitSemaphores);
  free(syncObjects.acquireSemaphores);
  free(syncObjects.frameFences);
}