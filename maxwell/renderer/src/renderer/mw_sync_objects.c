#include "mw_sync_objects.h"

#include <mw_core.h>

mw_sync_objects mw_renderer_create_sync_objects(
    mw_device device, const unsigned int maxFramesInFlight,
    const unsigned int swapchainImageCount) {
  mw_sync_objects syncObjects;
  syncObjects.frames_in_flight = maxFramesInFlight;
  syncObjects.swapchain_images = swapchainImageCount;

  syncObjects.acquire_semaphores =
      malloc(maxFramesInFlight * sizeof(VkSemaphore));
  syncObjects.submit_semaphores =
      malloc(swapchainImageCount * sizeof(VkSemaphore));
  syncObjects.frame_fences = malloc(maxFramesInFlight * sizeof(VkFence));

  VkSemaphoreCreateInfo semaphoreCreateInfo = {0};
  semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceCreateInfo = {0};
  fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < maxFramesInFlight || i < swapchainImageCount; i++) {
    if (i < swapchainImageCount)
      MW_ASSERT(vkCreateSemaphore(device.device, &semaphoreCreateInfo, NULL,
                                  syncObjects.submit_semaphores + i) ==
                VK_SUCCESS);
    if (i < maxFramesInFlight)
      MW_ASSERT(vkCreateSemaphore(device.device, &semaphoreCreateInfo, NULL,
                                  syncObjects.acquire_semaphores + i) ==
                    VK_SUCCESS &&
                vkCreateFence(device.device, &fenceCreateInfo, NULL,
                              syncObjects.frame_fences + i) == VK_SUCCESS);
  }

  return syncObjects;
}

void mw_renderer_destroy_sync_objects(mw_device device,
                                      mw_sync_objects syncObjects) {
  for (size_t i = 0;
       i < syncObjects.frames_in_flight || i < syncObjects.swapchain_images;
       i++) {
    if (i < syncObjects.swapchain_images)
      vkDestroySemaphore(device.device, syncObjects.submit_semaphores[i], NULL);
    if (i < syncObjects.frames_in_flight) {
      vkDestroySemaphore(device.device, syncObjects.acquire_semaphores[i],
                         NULL);
      vkDestroyFence(device.device, syncObjects.frame_fences[i], NULL);
    }
  }

  free(syncObjects.submit_semaphores);
  free(syncObjects.acquire_semaphores);
  free(syncObjects.frame_fences);
}