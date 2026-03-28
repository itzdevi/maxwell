#include "mw_application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "mw_core.h"
#include "mw_renderer.h"

GLFWwindow* window;

void mw_framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
  mw_renderer_update_framebuffer_resized();
}

void mw_app_start(mw_app_info info) {
  mw_vertex vertices[] = {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                          {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                          {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                          {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};
  uint16_t indices[] = {0, 1, 2, 2, 3, 0};

  MW_ASSERT(glfwInit());

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window = glfwCreateWindow(info.size[0], info.size[1], info.title, NULL, NULL);
  MW_ASSERT(window);

  glfwSetFramebufferSizeCallback(window, mw_framebuffer_resize_callback);

  mw_renderer_initialize(info, vertices, sizeof(vertices), indices,
                         sizeof(indices));

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

void mw_app_get_framebuffer_size(vec2 dest) {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  dest[0] = width;
  dest[1] = height;
}

void mw_app_wait_events() { glfwWaitEvents(); }