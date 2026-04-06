#include "mw_mesh.h"

#include <stdlib.h>
#include <string.h>

#include "core/mw_renderer_def.h"
#include "mw_index_buffer.h"
#include "mw_vertex_buffer.h"

uint32_t mw_renderer_add_mesh(mw_renderer* renderer, mw_vertex* vertices,
                              uint32_t verticesCount, uint32_t* indices,
                              uint32_t indicesCount) {
  renderer->meshPool = realloc(
      renderer->meshPool, (renderer->meshCount + 1) * sizeof(mw_mesh_entry));
  renderer->meshPool[renderer->meshCount] = (mw_mesh_entry){
      .verticesCount = verticesCount, .indicesCount = indicesCount};

  renderer->meshPool[renderer->meshCount].vertices =
      malloc(verticesCount * sizeof(mw_vertex));
  renderer->meshPool[renderer->meshCount].indices =
      malloc(indicesCount * sizeof(uint32_t));

  memcpy(renderer->meshPool[renderer->meshCount].vertices, vertices,
         verticesCount * sizeof(mw_vertex));
  memcpy(renderer->meshPool[renderer->meshCount].indices, indices,
         indicesCount * sizeof(uint32_t));

  return renderer->meshCount++;
}

void mw_renderer_destroy_mesh_pool(mw_renderer* renderer) {
  for (size_t i = 0; i < renderer->meshCount; i++) {
    free(renderer->meshPool[i].vertices);
    free(renderer->meshPool[i].indices);
  }

  free(renderer->meshPool);
}

void mw_renderer_create_meshes(mw_renderer* renderer) {
  if (renderer->meshCount == 0) return;

  renderer->meshes = malloc(renderer->meshCount * sizeof(mw_mesh));

  uint32_t vertexOffset = 0;
  uint32_t indexOffset = 0;

  for (size_t i = 0; i < renderer->meshCount; i++) {
    uint32_t verticesCount = renderer->meshPool[i].verticesCount;
    uint32_t indicesCount = renderer->meshPool[i].indicesCount;

    mw_mesh mesh = {.verticesOffset = vertexOffset,
                    .verticesCount = verticesCount,
                    .indicesOffset = indexOffset,
                    .indicesCount = indicesCount};
    renderer->meshes[i] = mesh;

    vertexOffset += verticesCount;
    indexOffset += indicesCount;
  }

  mw_renderer_create_mesh_buffers(renderer);
}

void mw_renderer_destroy_meshes(mw_renderer* renderer) {
  mw_renderer_destroy_buffer(renderer, renderer->vertexBuffer);
  mw_renderer_destroy_buffer(renderer, renderer->indexBuffer);
  free(renderer->meshes);
}

void mw_renderer_create_mesh_buffers(mw_renderer* renderer) {
  mw_mesh lastMesh = renderer->meshes[renderer->meshCount - 1];
  uint32_t totalVerts = lastMesh.verticesOffset + lastMesh.verticesCount;
  uint32_t totalIdx = lastMesh.indicesOffset + lastMesh.indicesCount;

  mw_vertex* flatVertices = malloc(totalVerts * sizeof(mw_vertex));
  uint32_t* flatIndices = malloc(totalIdx * sizeof(uint32_t));

  uint32_t idxVertices = 0;
  uint32_t idxIndices = 0;
  for (size_t i = 0; i < renderer->meshCount; i++) {
    for (size_t j = 0; j < renderer->meshes[i].verticesCount; j++) {
      flatVertices[idxVertices++] = renderer->meshPool[i].vertices[j];
    }

    for (size_t j = 0; j < renderer->meshes[i].indicesCount; j++) {
      flatIndices[idxIndices++] = renderer->meshPool[i].indices[j];
    }
  }

  void* p;
  renderer->vertexBuffer =
      mw_renderer_create_vertex_buffer(renderer, flatVertices, totalVerts, p);
  renderer->indexBuffer =
      mw_renderer_create_index_buffer(renderer, flatIndices, totalIdx, p);

  free(flatVertices);
  free(flatIndices);
}

void mw_renderer_destroy_mesh_buffers(mw_renderer* renderer) {}