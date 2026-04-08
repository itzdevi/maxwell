#ifndef MW_MESH_RENDERER_H
#define MW_MESH_RENDERER_H

#include <flecs.h>
#include <stdint.h>

#include "mw_engine.h"

typedef struct mw_mesh_renderer {
  uint32_t mesh;
} mw_mesh_renderer;

void mw_engine_system_draw_mesh(ecs_iter_t* it);

#endif