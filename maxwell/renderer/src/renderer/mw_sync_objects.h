#ifndef MW_RENDER_SYNC_H
#define MW_RENDER_SYNC_H

#include <mw_gpu.h>
#include <vulkan/vulkan.h>

typedef struct mw_sync_objects {
  VkFence* frameFences;
  VkSemaphore* acquireSemaphores;
  VkSemaphore* submitSemaphores;
  uint8_t framesInFlight;
  uint8_t swapchainImages;
} mw_sync_objects;

mw_sync_objects mw_renderer_create_sync_objects(
    mw_device device, const unsigned int maxFramesInFlight,
    const unsigned int swapchainImageCount);
void mw_renderer_destroy_sync_objects(mw_device device,
                                      mw_sync_objects syncObjects);

#endif