#ifndef MW_APPLICATION_H
#define MW_APPLICATION_H

#include <vulkan/vulkan.h>

#include "mw_math.h"

typedef struct mw_app_info {
  const char* title;
  const mw_vec2 size;
} mw_app_info;

void mw_app_start(mw_app_info info);
const char** mw_app_get_extensions(int* extensionCount);
VkResult mw_app_create_surface(VkInstance instance, VkSurfaceKHR* surface);
mw_vec2 mw_app_get_framebuffer_size();
void mw_app_wait_events();

#endif