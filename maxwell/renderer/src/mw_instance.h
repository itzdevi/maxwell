#ifndef MW_INSTANCE_H
#define MW_INSTANCE_H

#include <vulkan/vulkan.h>

VkInstance mw_renderer_create_instance(const char* title);
void mw_renderer_destroy_instance(VkInstance instance);

#endif