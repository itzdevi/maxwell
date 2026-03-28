#include "mw_application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "mw_core.h"
#include "mw_renderer.h"

GLFWwindow* window;
mw_renderer* renderer;

void mw_app_get_framebuffer_size(vec2 dest) {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  dest[0] = width;
  dest[1] = height;
}

void mw_framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
  vec2 fbSize = {width, height};
  mw_renderer_set_framebuffer_size(renderer, fbSize);
}

const char** mw_get_extensions(int* extensionCount) {
  return glfwGetRequiredInstanceExtensions(extensionCount);
}

VkResult mw_app_create_surface(VkInstance instance, VkSurfaceKHR* surface) {
  return glfwCreateWindowSurface(instance, window, NULL, surface);
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

  int extensionsCount = 0;
  const char** extensions = mw_get_extensions(&extensionsCount);
  mw_renderer_config rendererConfig = {
      .title = info.title,
      .extensionCount = extensionsCount,
      .extensions = extensions,
      .surfaceCreator = &mw_app_create_surface};
  glm_vec2_copy(info.size, rendererConfig.framebufferSize);
  renderer = mw_renderer_initialize(rendererConfig, vertices, sizeof(vertices),
                                    indices, sizeof(indices));

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    vec2 fbSize;
    mw_app_get_framebuffer_size(fbSize);
    if (fbSize[0] == 0 || fbSize[1] == 0) glfwWaitEvents();

    mw_renderer_draw_frame(renderer);
  }
  mw_renderer_wait_for_idle(renderer);

  glfwDestroyWindow(window);
  glfwTerminate();
}
