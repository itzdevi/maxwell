#ifndef MW_PHYSICAL_DEVICE_H
#define MW_PHYSICAL_DEVICE_H

#include <vulkan/vulkan.h>

typedef struct mw_swapchain_support_details {
  VkSurfaceCapabilitiesKHR capabilities;
  uint32_t formatsCount;
  VkSurfaceFormatKHR* formats;
  uint32_t presentModesCount;
  VkPresentModeKHR* presentModes;
} mw_swapchain_support_details;

mw_swapchain_support_details mw_gpu_create_swapchain_support_details(
    VkPhysicalDevice device, VkSurfaceKHR surface);
void mw_gpu_free_swapchain_support_details(
    mw_swapchain_support_details details);

VkPhysicalDevice mw_gpu_pick_physical_device(VkInstance instance,
                                             VkSurfaceKHR surface);

#endif