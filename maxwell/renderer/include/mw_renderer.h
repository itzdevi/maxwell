#ifndef MW_RENDERER_H
#define MW_RENDERER_H

#include <cglm/cglm.h>
#include <mw_gpu.h>

#include "mw_renderer_vertex.h"

typedef VkResult (*mw_renderer_surface_creator)(VkInstance, VkSurfaceKHR*,
                                                void*);

typedef struct mw_renderer_config {
  char* title;
  vec2 framebuffer_size;
  const char** extensions;
  uint32_t extension_count;
  mw_renderer_surface_creator surface_creator;
  void* window;
} mw_renderer_config;

typedef struct mw_render_pass {
  uint32_t mesh_index;
  mat4 transform;
} mw_render_pass;

typedef struct mw_renderer mw_renderer;

mw_renderer* mw_renderer_initialize(mw_renderer_config config);
void mw_renderer_submit_meshes(mw_renderer* renderer);
void mw_renderer_destroy(mw_renderer* renderer);
void mw_renderer_draw_frame(mw_renderer* renderer);
void mw_renderer_wait_for_idle(mw_renderer* renderer);
void mw_renderer_set_framebuffer_size(mw_renderer* renderer, vec2 size);
void mw_renderer_push_render_pass(mw_renderer* renderer,
                                  mw_render_pass render_pass);
uint32_t mw_renderer_add_mesh(mw_renderer* renderer, mw_vertex* vertices,
                              uint32_t vertices_count, uint32_t* indices,
                              uint32_t indices_count);

#endif