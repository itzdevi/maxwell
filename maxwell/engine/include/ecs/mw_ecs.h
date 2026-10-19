#ifndef MW_ECS_H
#define MW_ECS_H

#include <stddef.h>
#include <stdint.h>

#include "mw_context.h"

typedef struct ecs_iter_t it;

typedef struct mw_comps {
  void** ptrs;
  uint64_t* ids;
  size_t count;
} mw_comps;

typedef void (*mw_system_callback)(mw_ctx* ctx, uint64_t entity,
                                   mw_comps* comps);

typedef void (*mw_system_iter_callback)(it* iter);

#define MW_SYSTEM_ITER_CREATE(ctx, name, callback, components) \
  mw_engine_add_system_iter(ctx, name, callback, components,   \
                            sizeof(components) / sizeof(uint64_t))

extern uint64_t MW_COMPONENT_POSITION;
extern uint64_t MW_COMPONENT_ROTATION;
extern uint64_t MW_COMPONENT_SCALE;
extern uint64_t MW_COMPONENT_VELOCITY;
extern uint64_t MW_COMPONENT_ANGULAR_VELOCITY;
extern uint64_t MW_COMPONENT_MESH_RENDERER;
extern uint64_t MW_COMPONENT_CAMERA;

void mw_engine_init_ecs(mw_ctx* ctx);
void mw_engine_ecs_update(mw_ctx* ctx, double dt);
uint64_t mw_engine_create_entity(mw_ctx* ctx);
void* mw_engine_entity_add_component(mw_ctx* ctx, uint64_t entity,
                                     uint64_t component);
mw_ctx* mw_engine_get_ctx(it* it);
void mw_engine_add_system_iter(mw_ctx* ctx, const char* name,
                               mw_system_iter_callback callback,
                               uint64_t* components, size_t count);
void mw_engine_add_system(mw_ctx* ctx, const char* name,
                          mw_system_callback callback, uint64_t* components,
                          size_t count);
void* mw_engine_get_field(it* it, size_t size, int index);
void* mw_engine_get_component(mw_ctx* ctx, uint64_t entity, uint64_t component);
void* mw_engine_get_component_it(it* it, uint64_t entity, uint64_t component);
uint32_t mw_engine_get_iter_count(it* it);

void mw_engine_set_main_camera_entity(mw_ctx* ctx, uint64_t entity);

#endif