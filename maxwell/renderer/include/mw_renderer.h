#ifndef MW_RENDERER_H
#define MW_RENDERER_H

#include <cglm/cglm.h>
#include <mw_gpu.h>

#include "mw_renderer_vertex.h"

typedef VkResult (*mw_renderer_surface_creator)(VkInstance, VkSurfaceKHR*);

typedef struct mw_renderer_config {
  char* title;
  vec2 framebufferSize;
  const char** extensions;
  uint32_t extensionCount;
  mw_renderer_surface_creator surfaceCreator;
} mw_renderer_config;

typedef struct mw_render_pass {
  uint32_t meshIndex;
  mat4 transform;
} mw_render_pass;

typedef struct mw_renderer mw_renderer;

mw_renderer* mw_renderer_initialize(mw_renderer_config config);
void mw_renderer_destroy(mw_renderer* renderer);
void mw_renderer_draw_frame(mw_renderer* renderer);
void mw_renderer_wait_for_idle(mw_renderer* renderer);
void mw_renderer_set_framebuffer_size(mw_renderer* renderer, vec2 size);
void mw_renderer_push_render_pass(mw_renderer* renderer,
                                  mw_render_pass renderPass);

#endif