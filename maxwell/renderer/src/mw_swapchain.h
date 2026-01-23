#ifndef MW_SWAPCHAIN_H
#define MW_SWAPCHAIN_H

#include <vulkan/vulkan.h>

typedef struct mw_swapchain_support_details {
  VkSurfaceCapabilitiesKHR capabilities;
  unsigned int formatsCount;
  VkSurfaceFormatKHR* formats;
  unsigned int presentModesCount;
  VkPresentModeKHR* presentModes;
} mw_swapchain_support_details;

typedef struct mw_swapchain {
  VkSwapchainKHR swapchain;
  unsigned int imageCount;
  VkImage* images;
  VkFormat imageFormat;
  VkExtent2D extent;
  VkImageView* imageViews;
} mw_swapchain;

mw_swapchain_support_details mw_renderer_create_swapchain_support_details(
    VkPhysicalDevice device, VkSurfaceKHR surface);
void mw_renderer_free_swapchain_support_details(
    mw_swapchain_support_details details);

mw_swapchain mw_renderer_create_swapchain(VkPhysicalDevice physicalDevice,
                                          VkSurfaceKHR surface,
                                          VkDevice device);
void mw_renderer_destroy_swapchain(VkDevice device, mw_swapchain swapchain);

#endif