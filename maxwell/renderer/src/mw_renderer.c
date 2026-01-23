#include "mw_renderer.h"

#include "mw_instance.h"
#include "mw_physical_device.h"
#include "mw_surface.h"

VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice physicalDevice;

void mw_renderer_initialize(mw_app_info info) {
  instance = mw_renderer_create_instance(info.title);
  surface = mw_renderer_create_surface(instance);
  physicalDevice = mw_renderer_pick_physical_device(instance, surface);
}

void mw_renderer_cleanup() {
  mw_renderer_destroy_instance(instance);
  mw_renderer_destroy_surface(instance, surface);
}