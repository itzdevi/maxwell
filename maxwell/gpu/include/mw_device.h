#ifndef MW_DEVICE_H
#define MW_DEVICE_H

#include <vulkan/vulkan.h>

typedef struct mw_device {
  VkDevice device;
  VkQueue presentQueue;
  VkQueue graphicsQueue;
  VkQueue transferQueue;

  uint32_t presentIndex;
  uint32_t graphicsIndex;
  uint32_t transferIndex;
} mw_device;

int mw_gpu_check_device_extension_support(VkPhysicalDevice device);
mw_device mw_gpu_create_device(VkPhysicalDevice physicalDevice,
                               VkSurfaceKHR surface);
void mw_gpu_destroy_device(mw_device device);

#endif