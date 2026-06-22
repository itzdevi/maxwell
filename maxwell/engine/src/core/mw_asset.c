#include "mw_asset.h"

#include "core/mw_engine_def.h"
#define CGLTF_IMPLEMENTATION
#include <mw_assert.h>

#include "core/mw_engine_renderer.h"
#include "lib/cgltf.h"

void mw_load_glb(mw_ctx* ctx, const char* uri) {
  cgltf_options options = {0};
  cgltf_data* data = NULL;
  cgltf_result parseResult = cgltf_parse_file(&options, uri, &data);
  cgltf_result loadResult = cgltf_load_buffers(&options, data, uri);

  if (parseResult == cgltf_result_success &&
      loadResult == cgltf_result_success) {
    for (size_t m = 0; m < data->meshes_count; m++) {
      cgltf_mesh mesh = data->meshes[m];
      for (size_t p = 0; p < mesh.primitives_count; p++) {
        cgltf_primitive prim = mesh.primitives[p];

        uint32_t verticesCount = prim.attributes[0].data->count;
        mw_vertex* vertices =
            (mw_vertex*)malloc(sizeof(mw_vertex) * verticesCount);
        uint32_t* indices =
            (uint32_t*)malloc(sizeof(uint32_t) * prim.indices->count);

        for (size_t i = 0; i < prim.attributes_count; i++) {
          cgltf_attribute attrib = prim.attributes[i];

          if (attrib.type == cgltf_attribute_type_position) {
            for (size_t j = 0; j < verticesCount; j++) {
              MW_ASSERT(cgltf_accessor_read_float(
                  attrib.data, j, &vertices[j].position[0], sizeof(vec3)));
              vertices[j].position[1] *= -1;
            }
          }
        }

        MW_ASSERT(cgltf_accessor_unpack_indices(
            prim.indices, indices, sizeof(uint32_t), prim.indices->count));

        mw_engine_renderer_add_mesh(ctx, vertices, verticesCount, indices,
                                    prim.indices->count);

        free(vertices);
        free(indices);
      }
    }
    cgltf_free(data);
  }
}