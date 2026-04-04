#include "mw_application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define CGLTF_IMPLEMENTATION
#include <cglm/cglm.h>
#include <cgltf.h>
#include <mw_core.h>

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
  mw_vertex vertices[] = {{.position = {-0.5, 0.5, 0}},
                          {.position = {0.0, -0.5, 0}},
                          {.position = {0.5, 0.5, 0}}};

  uint32_t indices[] = {0, 1, 2};

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

  renderer = mw_renderer_initialize(rendererConfig);

  uint32_t triangle = mw_renderer_add_mesh(renderer, vertices, sizeof(vertices),
                                           indices, sizeof(indices));

  mw_renderer_submit_meshes(renderer);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    vec2 fbSize;
    mw_app_get_framebuffer_size(fbSize);
    if (fbSize[0] == 0 || fbSize[1] == 0) glfwWaitEvents();

    mat4 transform;

    glm_mat4_identity(transform);
    glm_translate(transform, (vec3){-1, 0, 0});
    mw_render_pass renderPass = {.meshIndex = triangle};
    glm_mat4_copy(transform, renderPass.transform);
    mw_renderer_push_render_pass(renderer, renderPass);

    glm_mat4_identity(transform);
    glm_translate(transform, (vec3){1, 0, 0});
    renderPass = (mw_render_pass){.meshIndex = triangle};
    glm_mat4_copy(transform, renderPass.transform);
    mw_renderer_push_render_pass(renderer, renderPass);

    mw_renderer_draw_frame(renderer);
  }
  mw_renderer_wait_for_idle(renderer);
  mw_renderer_destroy(renderer);

  glfwDestroyWindow(window);
  glfwTerminate();
}
