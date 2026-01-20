#include "mw_surface.h"

#include "mw_application.h"
#include "mw_core.h"

#include <stdio.h>

VkSurfaceKHR mw_renderer_create_surface(VkInstance instance) {
  VkSurfaceKHR surface;
  MW_ASSERT(mw_app_create_surface(instance, &surface) == VK_SUCCESS);
  return surface;
}

void mw_renderer_destroy_surface(VkInstance instance, VkSurfaceKHR surface) {
  vkDestroySurfaceKHR(instance, surface, NULL);
}