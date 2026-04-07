#ifndef MW_MESH_H
#define MW_MESH_H

#include <stdint.h>

#include "mw_renderer.h"

typedef struct mw_mesh {
  uint32_t vertices_offset;
  uint32_t vertices_count;
  uint32_t indices_offset;
  uint32_t indices_count;
} mw_mesh;

typedef struct mw_mesh_entry {
  mw_vertex* vertices;
  uint32_t vertices_count;
  uint32_t* indices;
  uint32_t indices_count;
} mw_mesh_entry;

uint32_t mw_renderer_add_mesh(mw_renderer* renderer, mw_vertex* vertices,
                              uint32_t verticesSize, uint32_t* indices,
                              uint32_t indicesSize);
void mw_renderer_destroy_mesh_pool(mw_renderer* renderer);
void mw_renderer_create_meshes(mw_renderer* renderer);
void mw_renderer_destroy_meshes(mw_renderer* renderer);
void mw_renderer_create_mesh_buffers(mw_renderer* renderer);
void mw_renderer_destroy_mesh_buffers(mw_renderer* renderer);

#endif