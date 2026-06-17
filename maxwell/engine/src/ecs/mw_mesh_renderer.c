#include "ecs/mw_mesh_renderer.h"

#include <cglm/io.h>
#include <mw_renderer.h>

#include "core/mw_engine_def.h"
#include "core/mw_window.h"
#include "ecs/mw_ecs.h"
#include "ecs/mw_transform.h"

void mw_engine_system_draw_mesh(mw_iter* it) {
  mw_ctx* ctx = mw_engine_get_ctx(it);
  const mw_position* pos = mw_engine_get_field(it, sizeof(mw_position), 0);
  const mw_rotation* rot = mw_engine_get_field(it, sizeof(mw_rotation), 1);
  const mw_scale* scale = mw_engine_get_field(it, sizeof(mw_scale), 2);
  const mw_mesh_renderer* meshRenderer =
      mw_engine_get_field(it, sizeof(mw_mesh_renderer), 3);

  for (size_t i = 0; i < it->count; i++) {
    mat4 transform;
    glm_mat4_identity(transform);
    glm_scale(transform, scale[i].data);
    glm_rotate(transform, rot[i].data[0], (vec3){1, 0, 0});
    glm_rotate(transform, rot[i].data[1], (vec3){0, 1, 0});
    glm_rotate(transform, rot[i].data[2], (vec3){0, 0, 1});
    glm_translate(transform, pos[i].data);

    const mw_camera* cam =
        mw_engine_get_component_it(it, ctx->main_camera_entity, MW_CAMERA);

    ivec2 windowSize;
    mw_engine_get_window_size(ctx, windowSize);

    mw_render_pass rp = {.mesh_index = meshRenderer->mesh};

    const mw_position* camPosition =
        mw_engine_get_component_it(it, ctx->main_camera_entity, MW_POSITION);
    glm_lookat(camPosition->data, GLM_VEC3_ZERO, GLM_YUP, rp.view);
    glm_vec3_copy(camPosition->data, rp.viewPos);

    glm_perspective(glm_rad(cam->fov), (float)windowSize[0] / windowSize[1],
                    cam->near_plane, cam->far_plane, rp.proj);

    glm_mat4_copy(transform, rp.transform);
    mw_renderer_push_render_pass(ctx->renderer, rp);
  }
}