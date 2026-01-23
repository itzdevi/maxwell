#ifndef MW_DEVICE_H
#define MW_DEVICE_H

#include <vulkan/vulkan.h>

int mw_renderer_check_device_extension_support(VkPhysicalDevice device);
VkDevice mw_renderer_create_device(VkPhysicalDevice physicalDevice,
                                   VkSurfaceKHR surface, VkQueue* presentQueue,
                                   VkQueue* graphicsQueue);
void mw_renderer_destroy_device(VkDevice device);

#endif