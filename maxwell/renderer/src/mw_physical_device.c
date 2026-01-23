#include "mw_physical_device.h"

#include "mw_assert.h"
#include "mw_device.h"
#include "mw_render_queue.h"
#include "mw_swapchain.h"

int is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
  struct mw_queue_family_indices indices =
      mw_renderer_find_queue_families(device, surface);

  int extensionsSupported = mw_renderer_check_device_extension_support(device);

  int swapChainAdequate = 0;
  if (extensionsSupported) {
    mw_swapchain_support_details details =
        mw_renderer_create_swapchain_support_details(device, surface);

    swapChainAdequate = details.formatsCount && details.presentModesCount;

    mw_renderer_free_swapchain_support_details(details);
  }

  return mw_renderer_check_queue_families(indices) && extensionsSupported &&
         swapChainAdequate;
}

VkPhysicalDevice mw_renderer_pick_physical_device(VkInstance instance,
                                                  VkSurfaceKHR surface) {
  unsigned int deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

  MW_ASSERT(deviceCount);

  VkPhysicalDevice physicalDevice;
  VkPhysicalDevice devices[deviceCount];
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

  for (size_t i = 0; i < deviceCount; i++) {
    if (is_device_suitable(devices[i], surface)) {
      physicalDevice = devices[i];
      break;
    }
  }

  MW_ASSERT(physicalDevice != VK_NULL_HANDLE);
  return physicalDevice;
}