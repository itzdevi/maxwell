#ifndef MW_PHYSICAL_DEVICE_H
#define MW_PHYSICAL_DEVICE_H

#include <vulkan/vulkan.h>

VkPhysicalDevice mw_renderer_pick_physical_device(VkInstance instance,
                                                  VkSurfaceKHR surface);

#endif