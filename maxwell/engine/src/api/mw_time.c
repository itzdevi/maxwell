#include "api/mw_time.h"

#include <flecs.h>

#include "core/mw_engine_def.h"
#include "core/mw_window.h"

mw_time mw_get_time(mw_ctx* ctx) {
    return (mw_time){.time = mw_engine_get_window_time(),
                   .delta_time = ecs_get_world_info(ctx->ecs)->delta_time};
}

mw_time mw_get_time_it(mw_iter* it) {
  return (mw_time){.time = mw_engine_get_window_time(),
                   .delta_time = it->delta_time};
}