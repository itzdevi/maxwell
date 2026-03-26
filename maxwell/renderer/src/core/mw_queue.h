#ifndef MW_RENDER_QUEUE_H
#define MW_RENDER_QUEUE_H

#include "vulkan/vulkan.h"

typedef struct mw_queue_family_indices {
  int graphicsFamily;
  int presentFamily;
  int transferFamily;
} mw_queue_family_indices;

mw_queue_family_indices mw_renderer_create_empty_queue_families();
int mw_renderer_check_queue_families(mw_queue_family_indices indices);
mw_queue_family_indices mw_renderer_find_queue_families(VkPhysicalDevice device,
                                                        VkSurfaceKHR surface);

#endif