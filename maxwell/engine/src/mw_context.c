#include "mw_context.h"

#include "core/mw_engine_def.h"
#include "core/mw_engine_renderer.h"
#include "core/mw_window.h"
#include "ecs/mw_ecs.h"
#include "ecs/mw_mesh_renderer.h"
#include "ecs/mw_transform.h"

mw_ctx* mw_engine_create(mw_engine_info info) {
  mw_ctx* ctx = malloc(sizeof(mw_ctx));

  mw_engine_create_window(ctx, &info);
  mw_engine_create_renderer(ctx, info);
  mw_engine_init_ecs(ctx);

  return ctx;
}

void mw_engine_run(mw_ctx* ctx) {
  mw_engine_renderer_finalize(ctx);

  double lastTime = mw_engine_get_window_time();
  while (!mw_engine_get_window_should_close(ctx)) {
    double currTime = mw_engine_get_window_time();
    double dt = currTime - lastTime;
    lastTime = currTime;

    mw_engine_window_update(ctx);

    mw_engine_ecs_update(ctx, dt);

    mw_engine_renderer_draw(ctx);
  }
}

void mw_engine_destroy(mw_ctx* ctx) {
  mw_engine_destroy_renderer(ctx);
  mw_engine_destroy_window(ctx);
  free(ctx);
}