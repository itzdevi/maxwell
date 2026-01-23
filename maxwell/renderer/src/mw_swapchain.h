#ifndef MW_SWAPCHAIN_H
#define MW_SWAPCHAIN_H

#include <vulkan/vulkan.h>

struct SwapchainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  unsigned int formatsCount;
  VkSurfaceFormatKHR* formats;
  unsigned int presentModesCount;
  VkPresentModeKHR* presentModes;
};

struct SwapchainSupportDetails mw_renderer_create_swapchain_support_details(
    VkPhysicalDevice device, VkSurfaceKHR surface);

void mw_renderer_free_swapchain_support_details(
    struct SwapchainSupportDetails details);

#endif