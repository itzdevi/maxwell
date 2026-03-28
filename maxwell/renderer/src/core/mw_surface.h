#ifndef MW_SURFACE_H
#define MW_SURFACE_H

#include <vulkan/vulkan.h>

#include "mw_renderer.h"

VkSurfaceKHR mw_renderer_create_surface(mw_renderer_config config,
                                        VkInstance instance);
void mw_renderer_destroy_surface(VkInstance instance, VkSurfaceKHR surface);

#endif