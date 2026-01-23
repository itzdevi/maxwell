#ifndef MW_RENDER_SYNC_H
#define MW_RENDER_SYNC_H

#include <vulkan/vulkan.h>

#include "mw_device.h"

typedef struct mw_sync_objects {
  VkSemaphore* imageAvailableSemaphores;
  VkSemaphore* renderFinishedSemaphores;
  VkFence* inFlightFences;
  unsigned int size;
} mw_sync_objects;

mw_sync_objects mw_renderer_create_sync_objects(
    mw_device device, const unsigned int maxFramesInFlight);
void mw_renderer_destroy_sync_objects(mw_device device,
                                      mw_sync_objects syncObjects);

#endif