#ifndef MW_INSTANCE_H
#define MW_INSTANCE_H

#include <vulkan/vulkan.h>

#include "mw_renderer.h"

VkInstance mw_renderer_create_instance(mw_renderer_config);
void mw_renderer_destroy_instance(VkInstance instance);

#endif