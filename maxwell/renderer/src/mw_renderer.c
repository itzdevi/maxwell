#include "mw_renderer.h"

#include "mw_device.h"
#include "mw_instance.h"
#include "mw_physical_device.h"
#include "mw_surface.h"
#include "mw_swapchain.h"

VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice physicalDevice;
VkDevice device;
VkQueue presentQueue;
VkQueue graphicsQueue;
VkSwapchainKHR swapchain;
int swapchainImageCount = 0;
VkImage* swapchainImages;
VkFormat format;
VkExtent2D swapchainExtent;

void mw_renderer_initialize(mw_app_info info) {
  instance = mw_renderer_create_instance(info.title);
  surface = mw_renderer_create_surface(instance);
  physicalDevice = mw_renderer_pick_physical_device(instance, surface);
  device = mw_renderer_create_device(physicalDevice, surface, &presentQueue,
                                     &graphicsQueue);
  swapchain = mw_renderer_create_swapchain(
      physicalDevice, surface, device, &swapchainImageCount, &swapchainImages,
      &format, &swapchainExtent);
}

void mw_renderer_cleanup() {
  mw_renderer_destroy_instance(instance);
  mw_renderer_destroy_device(device);
  mw_renderer_destroy_surface(instance, surface);
  mw_renderer_destroy_swapchain(device, swapchain, swapchainImages);
}