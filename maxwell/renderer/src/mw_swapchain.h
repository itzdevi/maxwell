#ifndef MW_SWAPCHAIN_H
#define MW_SWAPCHAIN_H

#include <vulkan/vulkan.h>

typedef struct mw_renderer_swapchain_support_details {
  VkSurfaceCapabilitiesKHR capabilities;
  unsigned int formatsCount;
  VkSurfaceFormatKHR* formats;
  unsigned int presentModesCount;
  VkPresentModeKHR* presentModes;
} mw_renderer_swapchain_support_details;

mw_renderer_swapchain_support_details
mw_renderer_create_swapchain_support_details(VkPhysicalDevice device,
                                             VkSurfaceKHR surface);
void mw_renderer_free_swapchain_support_details(
    mw_renderer_swapchain_support_details details);

VkSwapchainKHR mw_renderer_create_swapchain(
    VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkDevice device,
    unsigned int* imageCount, VkImage** images, VkFormat* imageFormat,
    VkExtent2D* extent);
void mw_renderer_destroy_swapchain(VkDevice device, VkSwapchainKHR swapchain,
                                   VkImage* images);

#endif