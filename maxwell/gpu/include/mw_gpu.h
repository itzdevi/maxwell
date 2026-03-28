#ifndef MW_GPU_H
#define MW_GPU_H

#include <vulkan/vulkan.h>

#include "mw_command.h"
#include "mw_device.h"
#include "mw_physical_device.h"
#include "mw_queue.h"

typedef struct mw_gpu_config {
  char* title;
  unsigned int extCount;
  const char** ext;
  VkSurfaceKHR surface;
} mw_gpu_config;

typedef struct mw_gpu_context {
  VkInstance instance;
  VkPhysicalDevice physicalDevice;
  mw_device device;
} mw_gpu_context;

VkInstance mw_gpu_create_instance(mw_gpu_config config);
mw_gpu_context mw_gpu_init(mw_gpu_config config, VkInstance instance);
void mw_gpu_destroy(mw_gpu_context ctx);

#endif