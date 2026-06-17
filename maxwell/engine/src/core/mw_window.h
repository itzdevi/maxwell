#ifndef MW_WINDOW_H
#define MW_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "mw_engine.h"

void mw_engine_create_window(mw_ctx* ctx, mw_engine_info* info);
void mw_engine_destroy_window(mw_ctx* ctx);
int mw_engine_get_window_should_close(mw_ctx* ctx);
void mw_engine_window_update(mw_ctx* ctx);
VkResult mw_engine_create_window_surface(VkInstance instance,
                                         VkSurfaceKHR* surface, void* window);
void mw_engine_get_window_size(mw_ctx* ctx, ivec2 a);
const char** mw_engine_get_window_extensions(int* extensionCount);
double mw_engine_get_window_time();

#endif