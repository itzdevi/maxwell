#include "ecs/mw_ecs.h"

#include "core/mw_engine_def.h"
#include "ecs/mw_camera.h"
#include "ecs/mw_mesh_renderer.h"
#include "ecs/mw_transform.h"

uint64_t MW_POSITION;
uint64_t MW_ROTATION;
uint64_t MW_SCALE;
uint64_t MW_VELOCITY;
uint64_t MW_ANGULAR_VELOCITY;
uint64_t MW_MESH_RENDERER;
uint64_t MW_CAMERA;

void mw_engine_init_ecs(mw_ctx* ctx) {
  ctx->ecs = ecs_init();
  ecs_set_ctx(ctx->ecs, ctx, NULL);

  ECS_COMPONENT(ctx->ecs, mw_position);
  ECS_COMPONENT(ctx->ecs, mw_rotation);
  ECS_COMPONENT(ctx->ecs, mw_scale);
  ECS_COMPONENT(ctx->ecs, mw_velocity);
  ECS_COMPONENT(ctx->ecs, mw_angular_velocity);
  ECS_COMPONENT(ctx->ecs, mw_mesh_renderer);
  ECS_COMPONENT(ctx->ecs, mw_camera);

  // Map Flecs internal IDs to our public variables
  MW_POSITION = ecs_id(mw_position);
  MW_ROTATION = ecs_id(mw_rotation);
  MW_SCALE = ecs_id(mw_scale);
  MW_VELOCITY = ecs_id(mw_velocity);
  MW_ANGULAR_VELOCITY = ecs_id(mw_angular_velocity);
  MW_MESH_RENDERER = ecs_id(mw_mesh_renderer);
  MW_CAMERA = ecs_id(mw_camera);

  mw_engine_add_system(
      ctx, "mesh_renderer", mw_engine_system_draw_mesh,
      (uint64_t[]){MW_POSITION, MW_ROTATION, MW_SCALE, MW_MESH_RENDERER}, 4);
}

void mw_engine_ecs_update(mw_ctx* ctx, double dt) {
  ecs_progress(ctx->ecs, (float)dt);
}

uint64_t mw_engine_create_entity(mw_ctx* ctx) { return ecs_new(ctx->ecs); }

void* mw_engine_entity_add_component(mw_ctx* ctx, uint64_t entity,
                                     uint64_t component) {
  ecs_add_id(ctx->ecs, entity, component);
  return (void*)ecs_get_id(ctx->ecs, entity, component);
}

mw_ctx* mw_engine_get_ctx(mw_iter* it) { return ecs_get_ctx(it->world); }

void mw_engine_add_system(mw_ctx* ctx, const char* name,
                          mw_system_callback callback, uint64_t* components,
                          size_t count) {
  ecs_system_desc_t desc = {0};
  desc.entity = ecs_entity(ctx->ecs, {.name = name});
  desc.phase = EcsOnUpdate;
  desc.callback = callback;

  for (size_t i = 0; i < count; i++) {
    desc.query.terms[i].id = components[i];
  }

  ecs_system_init(ctx->ecs, &desc);
}

void* mw_engine_get_field(mw_iter* it, size_t size, int index) {
  return ecs_field_w_size(it, size, index);
}

void* mw_engine_get_component(mw_ctx* ctx, uint64_t entity,
                              uint64_t component) {
  return (void*)ecs_get_id(ctx->ecs, entity, component);
}

void* mw_engine_get_component_it(mw_iter* it, uint64_t entity,
                                 uint64_t component) {
  return (void*)ecs_get_id(it->world, entity, component);
}

uint32_t mw_engine_get_iter_count(mw_iter* it) { return it->count; }

void mw_engine_set_main_camera_entity(mw_ctx* ctx, uint64_t entity) {
  ctx->main_camera_entity = entity;
}