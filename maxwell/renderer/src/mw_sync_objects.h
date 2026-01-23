#ifndef MW_RENDER_SYNC_H
#define MW_RENDER_SYNC_H

#include <vulkan/vulkan.h>

#include "mw_device.h"

typedef struct mw_sync_objects {
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
} mw_sync_objects;

mw_sync_objects mw_renderer_create_sync_objects(mw_device device);
void mw_renderer_destroy_sync_objects(mw_device device, mw_sync_objects syncObjects);

#endif