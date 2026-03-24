#include "mw_application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "mw_assert.h"
#include "mw_renderer.h"

GLFWwindow* window;

void mw_framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
  mw_renderer_update_framebuffer_resized();
}

void mw_app_start(mw_app_info info) {
  mw_vertex vertices[] = {{{-0.5, 0.5}, {1, 1, 1}},
                          {{0.5, 0.5}, {1, 1, 1}},
                          {{0, -0.5}, {1, 1, 1}}};

  MW_ASSERT(glfwInit());

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window = glfwCreateWindow(info.size.x, info.size.y, info.title, NULL, NULL);
  MW_ASSERT(window);

  glfwSetFramebufferSizeCallback(window, mw_framebuffer_resize_callback);

  mw_renderer_initialize(info, vertices, sizeof(vertices));

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    mw_renderer_draw_frame();
  }
  mw_renderer_wait_for_idle();

  glfwDestroyWindow(window);
  glfwTerminate();
}

const char** mw_app_get_extensions(int* extensionCount) {
  return glfwGetRequiredInstanceExtensions(extensionCount);
}

VkResult mw_app_create_surface(VkInstance instance, VkSurfaceKHR* surface) {
  return glfwCreateWindowSurface(instance, window, NULL, surface);
}

mw_vec2 mw_app_get_framebuffer_size() {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  return (mw_vec2){width, height};
}

void mw_app_wait_events() { glfwWaitEvents(); }