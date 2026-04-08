#ifndef MW_ENGINE_RENDERER_H
#define MW_ENGINE_RENDERER_H

#include "mw_engine.h"

void mw_engine_create_renderer(mw_ctx* ctx, mw_engine_info info);
void mw_engine_destroy_renderer(mw_ctx* ctx);
void mw_engine_renderer_draw(mw_ctx* ctx);

#endif