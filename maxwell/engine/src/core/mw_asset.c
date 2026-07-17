#include "mw_asset.h"

#include "core/mw_engine_def.h"
#define CGLTF_IMPLEMENTATION
#include <cglm/cglm.h>
#include <cglm/io.h>
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
                  attrib.data, j, vertices[j].position, sizeof(vec3)));
              vertices[j].position[1] *= -1;
            }
          }

          if (attrib.type == cgltf_attribute_type_normal) {
            for (size_t j = 0; j < verticesCount; j++) {
              MW_ASSERT(cgltf_accessor_read_float(
                  attrib.data, j, vertices[j].normal, sizeof(vec3)));
              vertices[j].normal[1] *= -1;
            }
          }

          if (attrib.type == cgltf_attribute_type_texcoord) {
            for (size_t j = 0; j < verticesCount; j++) {
              MW_ASSERT(cgltf_accessor_read_float(
                  attrib.data, j, vertices[j].uv, sizeof(vec2)));
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

    for (size_t n = 0; n < data->nodes_count; n++) {
      cgltf_node node = data->nodes[n];

      mat4 worldMat;
      cgltf_node_transform_world(&node, *worldMat);

      alignas(16) mw_position objPos;
      mat4 objRotMat;
      mw_scale objScale;
      glm_decompose(worldMat, objPos, objRotMat, objScale);
      mw_rotation objRot;
      glm_mat4_quat(objRotMat, objRot);
      objPos[1] *= -1;

      uint64_t e = mw_engine_create_entity(ctx);

      mw_position* pos = mw_engine_entity_add_component(ctx, e, MW_POSITION);
      glm_vec3_copy(objPos, *pos);

      mw_rotation* rot = mw_engine_entity_add_component(ctx, e, MW_ROTATION);
      glm_vec4_ucopy(objRot, *rot);

      mw_scale* scale = mw_engine_entity_add_component(ctx, e, MW_SCALE);
      glm_vec3_copy(objScale, *scale);

      mw_mesh_renderer* mr =
          mw_engine_entity_add_component(ctx, e, MW_MESH_RENDERER);
      mr->mesh_id = node.mesh - data->meshes;
    }
    cgltf_free(data);
  }
}