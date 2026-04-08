#include "mw_engine_renderer.h"

#include <mw_renderer.h>

#include "core/mw_engine_def.h"
#include "core/mw_window.h"

void mw_engine_create_renderer(mw_ctx* ctx, mw_engine_info info) {
  uint32_t extCount;
  const char** extensions = mw_engine_get_window_extensions(&extCount);

  mw_renderer_config cfg = {.title = info.title,
                            .window = ctx->window,
                            .surface_creator = mw_engine_create_window_surface,
                            .extensions = extensions,
                            .extension_count = extCount};
  glm_vec2_copy(info.size, cfg.framebuffer_size);
  ctx->renderer = mw_renderer_initialize(cfg);
}

void mw_engine_destroy_renderer(mw_ctx* ctx) {
  mw_renderer_destroy(ctx->renderer);
}

void mw_engine_renderer_draw(mw_ctx* ctx) {
  mw_renderer_draw_frame(ctx->renderer);
}