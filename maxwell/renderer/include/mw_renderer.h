#ifndef MW_RENDERER_H
#define MW_RENDERER_H

#include <cglm/cglm.h>

#include "mw_renderer_vertex.h"

typedef VkResult (*mw_renderer_surface_creator)(VkInstance, VkSurfaceKHR*);

typedef struct mw_renderer_config {
  char* title;
  vec2 framebufferSize;
  const char** extensions;
  uint32_t extensionCount;
  mw_renderer_surface_creator surfaceCreator;
} mw_renderer_config;

void mw_renderer_initialize(mw_renderer_config config, mw_vertex* vertices,
                            uint32_t verticesSize, uint16_t* indices,
                            uint32_t indicesSize);
void mw_renderer_cleanup();
void mw_renderer_draw_frame();
void mw_renderer_wait_for_idle();
void mw_renderer_set_framebuffer_size(vec2 size);

#endif