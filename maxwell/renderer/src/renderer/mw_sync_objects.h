#ifndef MW_RENDER_SYNC_H
#define MW_RENDER_SYNC_H

#include <mw_gpu.h>
#include <vulkan/vulkan.h>

#include "core/mw_renderer_def.h"

mw_sync_objects mw_renderer_create_sync_objects(
    mw_device device, const unsigned int maxFramesInFlight,
    const unsigned int swapchainImageCount);
void mw_renderer_destroy_sync_objects(mw_device device,
                                      mw_sync_objects syncObjects);

#endif