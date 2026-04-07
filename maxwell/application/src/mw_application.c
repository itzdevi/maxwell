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
double lastTime = 0;

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
  mw_vertex vertices[] = {{.position = {-0.5, -0.5, 0}},
                          {.position = {0.0, 0.5, 0}},
                          {.position = {0.5, -0.5, 0}}};

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
      .extension_count = extensionsCount,
      .extensions = extensions,
      .surface_creator = &mw_app_create_surface};
  glm_vec2_copy(info.size, rendererConfig.framebuffer_size);

  renderer = mw_renderer_initialize(rendererConfig);

  cgltf_options options = {0};
  cgltf_data* data;
  MW_ASSERT(cgltf_parse_file(&options, "../assets/models/ABeautifulGame.glb",
                             &data) == cgltf_result_success);
  MW_ASSERT(cgltf_load_buffers(&options, data,
                               "../assets/models/ABeautifulGame.glb") ==
            cgltf_result_success);

  uint32_t modelCount = data->meshes_count;
  uint32_t* models = malloc(sizeof(uint32_t) * modelCount);
  for (size_t i = 0; i < data->meshes_count; i++) {
    cgltf_mesh* mesh = &data->meshes[i];
    for (size_t j = 0; j < mesh->primitives_count; j++) {
      cgltf_primitive* prim = &mesh->primitives[j];

      // load vertices
      uint32_t vertexCount = prim[0].attributes->data->count;
      mw_vertex* vertices = malloc(sizeof(mw_vertex) * vertexCount);
      for (size_t k = 0; k < prim->attributes_count; k++) {
        cgltf_attribute* attr = &prim->attributes[k];

        if (attr->type == cgltf_attribute_type_position) {
          for (size_t v = 0; v < vertexCount; v++) {
            cgltf_accessor_read_float(attr->data, v, vertices[v].position, 3);
          }
        }

        if (attr->type == cgltf_attribute_type_normal) {
          for (size_t v = 0; v < vertexCount; v++) {
            cgltf_accessor_read_float(attr->data, v, vertices[v].normal, 3);
          }
        }
      }

      // load indices
      uint32_t indexCount = prim->indices->count;
      uint32_t* indices = malloc(sizeof(uint32_t) * indexCount);
      for (size_t v = 0; v < indexCount; v++) {
        indices[v] = cgltf_accessor_read_index(prim->indices, v);
      }

      models[i] = mw_renderer_add_mesh(renderer, vertices, vertexCount, indices,
                                       indexCount);
      free(vertices);
      free(indices);
    }
  }

  if (info.on_load) info.on_load();

  mw_renderer_submit_meshes(renderer);

  lastTime = glfwGetTime();
  uint8_t frame;
  double rot = 0;
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    double dt = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();

    vec2 fbSize;
    mw_app_get_framebuffer_size(fbSize);
    if (fbSize[0] == 0 || fbSize[1] == 0) glfwWaitEvents();

    for (size_t i = 0; i < data->nodes_count; i++) {
      cgltf_node* node = &data->nodes[i];

      uint32_t meshIndex = models[node->mesh - data->meshes];
      float worldMat[16];
      cgltf_node_transform_world(&data->nodes[i], worldMat);
      mat4 model = {{worldMat[0], worldMat[1], worldMat[2], worldMat[3]},
                    {worldMat[4], worldMat[5], worldMat[6], worldMat[7]},
                    {worldMat[8], worldMat[9], worldMat[10], worldMat[11]},
                    {worldMat[12], worldMat[13], worldMat[14], worldMat[15]}};
      mat4 transMat;
      glm_mat4_identity(transMat);
      glm_translate(transMat, (vec3){0, -1, 0});
      glm_rotate(transMat, rot, (vec3){0, 1, 0});
      glm_scale(transMat, (vec3){4, 4, 4});
      glm_mul(transMat, model, model);
      mw_render_pass rp = {.mesh_index = meshIndex};
      glm_mat4_copy(model, rp.transform);
      mw_renderer_push_render_pass(renderer, rp);
    }

    // mat4 transform;
    // glm_mat4_identity(transform);
    // glm_translate(transform, (vec3){0, -1, 0});
    // glm_rotate(transform, rot, (vec3){0, 1, 0});
    // glm_scale(transform, (vec3){10, 10, 10});
    // glm_scale(transform, (vec3){0.1f, 0.1f, 0.1f});

    // mw_render_pass rp = {.meshIndex = 0};
    // glm_mat4_copy(transform, rp.transform);
    // mw_renderer_push_render_pass(renderer, rp);

    rot += dt;

    // glm_mat4_identity(transform);
    // glm_translate(transform, (vec3){1, 0, 0});
    // renderPass = (mw_render_pass){.meshIndex = triangle};
    // glm_mat4_copy(transform, renderPass.transform);
    // mw_renderer_push_render_pass(renderer, renderPass);

    mw_renderer_draw_frame(renderer);
    frame++;
  }
  mw_renderer_wait_for_idle(renderer);
  mw_renderer_destroy(renderer);

  free(models);
  cgltf_free(data);

  glfwDestroyWindow(window);
  glfwTerminate();
}
