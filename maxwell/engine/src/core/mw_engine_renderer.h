#ifndef MW_ENGINE_RENDERER_H
#define MW_ENGINE_RENDERER_H

#include <mw_renderer.h>
#include <stdint.h>

#include "mw_engine.h"

void mw_engine_create_renderer(mw_ctx* ctx, mw_engine_info info);
void mw_engine_destroy_renderer(mw_ctx* ctx);
void mw_engine_renderer_draw(mw_ctx* ctx);
void mw_engine_renderer_finalize(mw_ctx* ctx);
void mw_engine_renderer_add_mesh(mw_ctx* ctx, mw_vertex* vertices,
                                 uint32_t vertex_count, uint32_t* indices,
                                 uint32_t index_count);

#endif