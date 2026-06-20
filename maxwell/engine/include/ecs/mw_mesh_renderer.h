#ifndef MW_MESH_RENDERER_H
#define MW_MESH_RENDERER_H

#include <stdint.h>

#include "ecs/mw_ecs.h"

typedef struct mw_mesh_renderer {
  uint32_t mesh_id;
} mw_mesh_renderer;

void mw_engine_system_draw_mesh(mw_iter* it);

#endif