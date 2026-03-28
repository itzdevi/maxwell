#include "mw_physical_device.h"

#include "mw_core.h"
#include "mw_device.h"
#include "mw_queue.h"

mw_swapchain_support_details mw_gpu_create_swapchain_support_details(
    VkPhysicalDevice device, VkSurfaceKHR surface) {
  mw_swapchain_support_details details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);

  unsigned int formatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, NULL);
  details.formatsCount = formatCount;

  if (formatCount) {
    details.formats = malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.formats);
  }

  unsigned int presentModeCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            NULL);
  details.presentModesCount = presentModeCount;

  if (presentModeCount) {
    details.presentModes = malloc(presentModeCount * sizeof(VkPresentModeKHR));
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes);
  }

  return details;
}

void mw_gpu_free_swapchain_support_details(
    mw_swapchain_support_details details) {
  free(details.formats);
  free(details.presentModes);
}

int is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
  struct mw_queue_family_indices indices =
      mw_gpu_find_queue_families(device, surface);

  int extensionsSupported = mw_gpu_check_device_extension_support(device);

  int swapChainAdequate = 0;
  if (extensionsSupported) {
    mw_swapchain_support_details details =
        mw_gpu_create_swapchain_support_details(device, surface);

    swapChainAdequate = details.formatsCount && details.presentModesCount;

    mw_gpu_free_swapchain_support_details(details);
  }

  return mw_gpu_check_queue_families(indices) && extensionsSupported &&
         swapChainAdequate;
}

VkPhysicalDevice mw_gpu_pick_physical_device(VkInstance instance,
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