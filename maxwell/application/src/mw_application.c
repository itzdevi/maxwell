#include "mw_application.h"

#include <GLFW/glfw3.h>

#include "mw_assert.h"
#include "mw_renderer.h"

GLFWwindow* window;

void mw_app_start(mw_app_info info) {
  MW_ASSERT(glfwInit());

  window = glfwCreateWindow(info.size.x, info.size.y, info.title, NULL, NULL);
  MW_ASSERT(window);

  mw_renderer_initialize(info);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}

const char** mw_app_get_extensions(int* extensionCount) {
  return glfwGetRequiredInstanceExtensions(extensionCount);
}

VkResult mw_app_create_surface(VkInstance instance, VkSurfaceKHR* surface) {
  return glfwCreateWindowSurface(instance, window, NULL, surface);
}