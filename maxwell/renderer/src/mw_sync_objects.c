#include "mw_sync_objects.h"

#include "mw_assert.h"

mw_sync_objects mw_renderer_create_sync_objects(
    mw_device device, const unsigned int maxFramesInFlight) {
  mw_sync_objects syncObjects;
  syncObjects.size = maxFramesInFlight;

  syncObjects.imageAvailableSemaphores =
      malloc(maxFramesInFlight * sizeof(VkSemaphore));
  syncObjects.renderFinishedSemaphores =
      malloc(maxFramesInFlight * sizeof(VkSemaphore));
  syncObjects.inFlightFences = malloc(maxFramesInFlight * sizeof(VkFence));

  VkSemaphoreCreateInfo semaphoreCreateInfo = {0};
  semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceCreateInfo = {0};
  fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < maxFramesInFlight; i++) {
    MW_ASSERT(vkCreateSemaphore(device.device, &semaphoreCreateInfo, NULL,
                                &syncObjects.imageAvailableSemaphores[i]) ==
                  VK_SUCCESS &&
              vkCreateSemaphore(device.device, &semaphoreCreateInfo, NULL,
                                &syncObjects.renderFinishedSemaphores[i]) ==
                  VK_SUCCESS &&
              vkCreateFence(device.device, &fenceCreateInfo, NULL,
                            &syncObjects.inFlightFences[i]) == VK_SUCCESS);
  }

  return syncObjects;
}

void mw_renderer_destroy_sync_objects(mw_device device,
                                      mw_sync_objects syncObjects) {
  for (size_t i = 0; i < syncObjects.size; i++) {
    vkDestroySemaphore(device.device, syncObjects.imageAvailableSemaphores[i],
                       NULL);
    vkDestroySemaphore(device.device, syncObjects.renderFinishedSemaphores[i],
                       NULL);
    vkDestroyFence(device.device, syncObjects.inFlightFences[i], NULL);
  }

  free(syncObjects.imageAvailableSemaphores);
  free(syncObjects.renderFinishedSemaphores);
  free(syncObjects.inFlightFences);
}