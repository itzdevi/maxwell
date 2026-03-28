#ifndef MW_SWAPCHAIN_H
#define MW_SWAPCHAIN_H

#include <cglm/cglm.h>
#include <mw_gpu.h>
#include <vulkan/vulkan.h>

typedef struct mw_swapchain {
  VkSwapchainKHR swapchain;
  unsigned int imageCount;
  VkImage* images;
  VkFormat imageFormat;
  VkExtent2D extent;
  VkImageView* imageViews;
} mw_swapchain;

mw_swapchain mw_renderer_create_swapchain(VkPhysicalDevice physicalDevice,
                                          VkSurfaceKHR surface,
                                          mw_device device,
                                          vec2 framebufferSize);
void mw_renderer_destroy_swapchain(mw_device device, mw_swapchain swapchain);

#endif