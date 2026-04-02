#ifndef MW_UNIFORM_BUFFER_H
#define MW_UNIFORM_BUFFER_H

#include <cglm/cglm.h>

#include "mw_buffer.h"
#include "mw_renderer.h"

typedef struct mw_mvp {
  mat4 model;
  mat4 view;
  mat4 proj;
} mw_mvp;

typedef struct mw_ubo {
  mw_buffer buffer;
  void* data;
} mw_ubo;

void mw_renderer_create_uniform_buffers(mw_renderer* renderer,
                                        uint32_t framesInFlight);
void mw_renderer_destroy_uniform_buffers(mw_renderer* renderer,
                                         uint32_t framesInFlight);
void mw_renderer_update_current_uniform_buffer(mw_renderer* renderer,
                                               mat4 transform);

#endif