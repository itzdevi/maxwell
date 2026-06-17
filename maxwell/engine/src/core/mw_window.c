#include "mw_window.h"

#include <GLFW/glfw3.h>
#include <mw_assert.h>

#include "core/mw_engine_def.h"

void mw_framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
  vec2 fbSize = {width, height};
  mw_ctx* ctx = glfwGetWindowUserPointer(window);
  mw_renderer_set_framebuffer_size(ctx->renderer, fbSize);
}

VkResult mw_engine_create_window_surface(VkInstance instance,
                                         VkSurfaceKHR* surface, void* window) {
  return glfwCreateWindowSurface(instance, (GLFWwindow*)window, NULL, surface);
}

void mw_engine_create_window(mw_ctx* ctx, mw_engine_info* info) {
  MW_ASSERT(glfwInit());

  int fullScreen = glm_vec2_eqv(info->size, GLM_VEC2_ZERO);
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);

  if (fullScreen)
    glm_vec2_copy((vec2){vidMode->width, vidMode->height}, info->size);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  ctx->window = glfwCreateWindow(info->size[0], info->size[1], info->title,
                                 fullScreen ? monitor : NULL, NULL);
  MW_ASSERT(ctx->window);

  glfwSetWindowUserPointer(ctx->window, ctx);
  glfwSetFramebufferSizeCallback(ctx->window, mw_framebuffer_resize_callback);
}

void mw_engine_destroy_window(mw_ctx* ctx) {
  glfwDestroyWindow(ctx->window);
  glfwTerminate();
}

int mw_engine_get_window_should_close(mw_ctx* ctx) {
  return glfwWindowShouldClose(ctx->window);
}

void mw_engine_window_update(mw_ctx* ctx) { glfwPollEvents(); }

void mw_engine_get_window_size(mw_ctx* ctx, ivec2 a) {
  glfwGetFramebufferSize(ctx->window, &a[0], &a[1]);
}

const char** mw_engine_get_window_extensions(int* extensionCount) {
  return glfwGetRequiredInstanceExtensions(extensionCount);
}

double mw_engine_get_window_time() { return glfwGetTime(); }
