#include "mw_surface.h"

#include <mw_core.h>
#include <stdio.h>

VkSurfaceKHR mw_renderer_create_surface(mw_renderer_config config,
                                        VkInstance instance) {
  VkSurfaceKHR surface;
  MW_ASSERT(config.surfaceCreator(instance, &surface) == VK_SUCCESS);
  return surface;
}

void mw_renderer_destroy_surface(VkInstance instance, VkSurfaceKHR surface) {
  vkDestroySurfaceKHR(instance, surface, NULL);
}