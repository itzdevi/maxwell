#ifndef MW_RENDER_SYNC_H
#define MW_RENDER_SYNC_H

#include <mw_gpu.h>
#include <vulkan/vulkan.h>

typedef struct mw_sync_objects {
  VkFence* frame_fences;
  VkSemaphore* acquire_semaphores;
  VkSemaphore* submit_semaphores;
  uint8_t frames_in_flight;
  uint8_t swapchain_images;
} mw_sync_objects;

mw_sync_objects mw_renderer_create_sync_objects(
    mw_device device, const unsigned int maxFramesInFlight,
    const unsigned int swapchainImageCount);
void mw_renderer_destroy_sync_objects(mw_device device,
                                      mw_sync_objects syncObjects);

#endif