#include "mw_ecs.h"

#include <flecs.h>

#include "core/mw_engine_def.h"
#include "ecs/mw_mesh_renderer.h"
#include "ecs/mw_transform.h"

void mw_engine_init_ecs(mw_ctx* ctx) {
  ctx->ecs = ecs_init();

  ECS_COMPONENT(ctx->ecs, mw_ctx);
  ECS_COMPONENT(ctx->ecs, mw_position);
  ECS_COMPONENT(ctx->ecs, mw_rotation);
  ECS_COMPONENT(ctx->ecs, mw_scale);
  ECS_COMPONENT(ctx->ecs, mw_velocity);
  ECS_COMPONENT(ctx->ecs, mw_angular_velocity);
  ECS_COMPONENT(ctx->ecs, mw_mesh_renderer);

  ECS_SYSTEM(ctx->ecs, mw_engine_system_draw_mesh, EcsOnUpdate, mw_position,
             mw_rotation, mw_scale, mw_mesh_renderer);
}