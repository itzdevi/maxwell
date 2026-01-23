#ifndef MW_RENDER_QUEUE_H
#define MW_RENDER_QUEUE_H

#include "vulkan/vulkan.h"

struct QueueFamilyIndices {
  int graphicsFamily;
  int presentFamily;
};

struct QueueFamilyIndices mw_renderer_create_empty_queue_families();
int mw_renderer_check_queue_families(struct QueueFamilyIndices indices);
struct QueueFamilyIndices mw_renderer_find_queue_families(
    VkPhysicalDevice device, VkSurfaceKHR surface);

#endif