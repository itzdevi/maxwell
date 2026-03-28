#ifndef MW_INSTANCE_H
#define MW_INSTANCE_H

#include <vulkan/vulkan.h>

#include "mw_gpu.h"

VkInstance mw_gpu_create_instance(mw_gpu_config config);
void mw_gpu_destroy_instance(VkInstance instance);

#endif