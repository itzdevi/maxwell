#include "mw_engine_renderer.h"

#include <mw_renderer.h>

#include "core/mw_engine_def.h"
#include "core/mw_window.h"
#include "mw_context.h"

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
  mw_renderer_submit_meshes(ctx->renderer);
}

void mw_engine_destroy_renderer(mw_ctx* ctx) {
  mw_renderer_destroy(ctx->renderer);
}

void mw_engine_renderer_draw(mw_ctx* ctx) {
  mw_renderer_draw_frame(ctx->renderer);
}

void mw_engine_renderer_finalize(mw_ctx* ctx) {
  mw_renderer_submit_meshes(ctx->renderer);
}

void mw_engine_renderer_add_mesh(mw_ctx* ctx, mw_vertex* vertices,
                                 uint32_t vertex_count, uint32_t* indices,
                                 uint32_t index_count) {
  mw_renderer_add_mesh(ctx->renderer, vertices, vertex_count, indices,
                       index_count);
}