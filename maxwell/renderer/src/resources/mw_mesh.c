#include "mw_mesh.h"

#include <stdlib.h>
#include <string.h>

#include "core/mw_renderer_def.h"
#include "mw_index_buffer.h"
#include "mw_vertex_buffer.h"

uint32_t mw_renderer_add_mesh(mw_renderer* renderer, mw_vertex* vertices,
                              uint32_t verticesCount, uint32_t* indices,
                              uint32_t indicesCount) {
  renderer->mesh_pool = realloc(
      renderer->mesh_pool, (renderer->mesh_count + 1) * sizeof(mw_mesh_entry));
  renderer->mesh_pool[renderer->mesh_count] = (mw_mesh_entry){
      .vertices_count = verticesCount, .indices_count = indicesCount};

  renderer->mesh_pool[renderer->mesh_count].vertices =
      malloc(verticesCount * sizeof(mw_vertex));
  renderer->mesh_pool[renderer->mesh_count].indices =
      malloc(indicesCount * sizeof(uint32_t));

  memcpy(renderer->mesh_pool[renderer->mesh_count].vertices, vertices,
         verticesCount * sizeof(mw_vertex));
  memcpy(renderer->mesh_pool[renderer->mesh_count].indices, indices,
         indicesCount * sizeof(uint32_t));

  return renderer->mesh_count++;
}

void mw_renderer_destroy_mesh_pool(mw_renderer* renderer) {
  for (size_t i = 0; i < renderer->mesh_count; i++) {
    free(renderer->mesh_pool[i].vertices);
    free(renderer->mesh_pool[i].indices);
  }

  free(renderer->mesh_pool);
}

void mw_renderer_create_meshes(mw_renderer* renderer) {
  if (renderer->mesh_count == 0) return;

  renderer->meshes = malloc(renderer->mesh_count * sizeof(mw_mesh));

  uint32_t vertexOffset = 0;
  uint32_t indexOffset = 0;

  for (size_t i = 0; i < renderer->mesh_count; i++) {
    uint32_t verticesCount = renderer->mesh_pool[i].vertices_count;
    uint32_t indicesCount = renderer->mesh_pool[i].indices_count;

    mw_mesh mesh = {.vertices_offset = vertexOffset,
                    .vertices_count = verticesCount,
                    .indices_offset = indexOffset,
                    .indices_count = indicesCount};
    renderer->meshes[i] = mesh;

    vertexOffset += verticesCount;
    indexOffset += indicesCount;
  }

  mw_renderer_create_mesh_buffers(renderer);
}

void mw_renderer_destroy_meshes(mw_renderer* renderer) {
  mw_renderer_destroy_buffer(renderer, renderer->vertex_buffer);
  mw_renderer_destroy_buffer(renderer, renderer->index_buffer);
  free(renderer->meshes);
}

void mw_renderer_create_mesh_buffers(mw_renderer* renderer) {
  mw_mesh lastMesh = renderer->meshes[renderer->mesh_count - 1];
  uint32_t totalVerts = lastMesh.vertices_offset + lastMesh.vertices_count;
  uint32_t totalIdx = lastMesh.indices_offset + lastMesh.indices_count;

  mw_vertex* flatVertices = malloc(totalVerts * sizeof(mw_vertex));
  uint32_t* flatIndices = malloc(totalIdx * sizeof(uint32_t));

  uint32_t idxVertices = 0;
  uint32_t idxIndices = 0;
  for (size_t i = 0; i < renderer->mesh_count; i++) {
    for (size_t j = 0; j < renderer->meshes[i].vertices_count; j++) {
      flatVertices[idxVertices++] = renderer->mesh_pool[i].vertices[j];
    }

    for (size_t j = 0; j < renderer->meshes[i].indices_count; j++) {
      flatIndices[idxIndices++] = renderer->mesh_pool[i].indices[j];
    }
  }

  void* p;
  renderer->vertex_buffer =
      mw_renderer_create_vertex_buffer(renderer, flatVertices, totalVerts, p);
  renderer->index_buffer =
      mw_renderer_create_index_buffer(renderer, flatIndices, totalIdx, p);

  free(flatVertices);
  free(flatIndices);
}

void mw_renderer_destroy_mesh_buffers(mw_renderer* renderer) {}