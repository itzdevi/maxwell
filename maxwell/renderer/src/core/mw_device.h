#ifndef MW_DEVICE_H
#define MW_DEVICE_H

#include <vulkan/vulkan.h>

typedef struct mw_device {
  VkDevice device;
  VkQueue presentQueue;
  VkQueue graphicsQueue;
  VkQueue transferQueue;
} mw_device;

int mw_renderer_check_device_extension_support(VkPhysicalDevice device);
mw_device mw_renderer_create_device(VkPhysicalDevice physicalDevice,
                                    VkSurfaceKHR surface);
void mw_renderer_destroy_device(mw_device device);

#endif