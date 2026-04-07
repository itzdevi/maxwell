#ifndef MW_SWAPCHAIN_H
#define MW_SWAPCHAIN_H

#include <cglm/cglm.h>
#include <mw_gpu.h>
#include <vulkan/vulkan.h>

#include "mw_renderer.h"

typedef struct mw_swapchain {
  VkSwapchainKHR swapchain;
  unsigned int image_count;
  VkImageView* image_views;
  VkImage* images;
  VkFormat image_format;
  VkImageView depth_image_view;
  VkImage depth_image;
  VkFormat depth_format;
  VkDeviceMemory depth_image_memory;
  VkExtent2D extent;
} mw_swapchain;

void mw_renderer_create_swapchain(mw_renderer* renderer);
void mw_renderer_destroy_swapchain(mw_renderer* renderer);

#endif