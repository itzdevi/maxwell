#include "mw_physical_device.h"

#include "mw_core.h"
#include "mw_device.h"
#include "mw_swapchain.h"

struct QueueFamilyIndices {
  int graphicsFamily;
  int presentFamily;
};

struct QueueFamilyIndices create_empty_queue_families() {
  struct QueueFamilyIndices indices = {-1, -1};
  return indices;
}

int check_queue_families(struct QueueFamilyIndices indices) {
  return indices.graphicsFamily != -1 && indices.presentFamily != -1;
}

struct QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                            VkSurfaceKHR surface) {
  struct QueueFamilyIndices indices = create_empty_queue_families();

  unsigned int queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

  VkQueueFamilyProperties queueFamilies[queueFamilyCount];
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies);

  for (size_t i = 0; i < queueFamilyCount; i++) {
    if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      indices.graphicsFamily = i;

    VkBool32 presentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    if (presentSupport) indices.presentFamily = i;

    if (check_queue_families(indices)) break;
  }

  return indices;
}

int is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
  struct QueueFamilyIndices indices = findQueueFamilies(device, surface);

  int extensionsSupported = mw_renderer_check_device_extension_support(device);

  int swapChainAdequate = 0;
  if (extensionsSupported) {
    struct SwapchainSupportDetails details =
        mw_renderer_create_swapchain_support_details(device, surface);

    swapChainAdequate = details.formatsCount && details.presentModesCount;

    mw_renderer_free_swapchain_support_details(details);
  }

  return check_queue_families(indices) && extensionsSupported &&
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