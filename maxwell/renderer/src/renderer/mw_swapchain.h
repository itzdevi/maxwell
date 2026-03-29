#ifndef MW_SWAPCHAIN_H
#define MW_SWAPCHAIN_H

#include <cglm/cglm.h>
#include <mw_gpu.h>
#include <vulkan/vulkan.h>

#include "mw_renderer.h"

typedef struct mw_swapchain {
  VkSwapchainKHR swapchain;
  unsigned int imageCount;
  VkImage* images;
  VkFormat imageFormat;
  VkExtent2D extent;
  VkImageView* imageViews;
} mw_swapchain;

void mw_renderer_create_swapchain(mw_renderer* renderer);
void mw_renderer_destroy_swapchain(mw_renderer* renderer);

#endif