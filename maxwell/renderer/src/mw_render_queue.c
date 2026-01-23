#include "mw_render_queue.h"

mw_queue_family_indices mw_renderer_create_empty_queue_families() {
  mw_queue_family_indices indices = {-1, -1};
  return indices;
}

int mw_renderer_check_queue_families(struct mw_queue_family_indices indices) {
  return indices.graphicsFamily != -1 && indices.presentFamily != -1;
}

mw_queue_family_indices mw_renderer_find_queue_families(VkPhysicalDevice device,
                                                        VkSurfaceKHR surface) {
  mw_queue_family_indices indices = mw_renderer_create_empty_queue_families();

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

    if (mw_renderer_check_queue_families(indices)) break;
  }

  return indices;
}