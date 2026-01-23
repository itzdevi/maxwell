#include "mw_swapchain.h"

#include "stdlib.h"

struct SwapchainSupportDetails mw_renderer_create_swapchain_support_details(
    VkPhysicalDevice device, VkSurfaceKHR surface) {
  struct SwapchainSupportDetails details;

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

void mw_renderer_free_swapchain_support_details(
    struct SwapchainSupportDetails details) {
  free(details.formats);
  free(details.presentModes);
}