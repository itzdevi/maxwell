#include "mw_window.h"

#include <GLFW/glfw3.h>
#include <mw_assert.h>

#include "core/mw_engine_def.h"

VkResult mw_engine_create_window_surface(VkInstance instance,
                                         VkSurfaceKHR* surface, void* window) {
  return glfwCreateWindowSurface(instance, (GLFWwindow*)window, NULL, surface);
}

void mw_engine_create_window(mw_ctx* ctx, mw_engine_info info) {
  MW_ASSERT(glfwInit());

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  ctx->window =
      glfwCreateWindow(info.size[0], info.size[1], info.title, NULL, NULL);
  MW_ASSERT(ctx->window);
}

void mw_engine_destroy_window(mw_ctx* ctx) {
  glfwDestroyWindow(ctx->window);
  glfwTerminate();
}

int mw_engine_get_window_should_close(mw_ctx* ctx) {
  return glfwWindowShouldClose(ctx->window);
}

void mw_engine_window_update(mw_ctx* ctx) { glfwPollEvents(); }

const char** mw_engine_get_window_extensions(int* extensionCount) {
  return glfwGetRequiredInstanceExtensions(extensionCount);
}