#include "ecs/mw_mesh_renderer.h"

#include <cglm/io.h>
#include <mw_renderer.h>

#include "core/mw_engine_def.h"
#include "core/mw_window.h"
#include "ecs/mw_ecs.h"
#include "ecs/mw_transform.h"

void mw_engine_system_draw_mesh(mw_iter* it) {
  mw_ctx* ctx = mw_engine_get_ctx(it);
  mw_position* pos = mw_engine_get_field(it, sizeof(mw_position), 0);
  mw_rotation* rot = mw_engine_get_field(it, sizeof(mw_rotation), 1);
  mw_scale* scale = mw_engine_get_field(it, sizeof(mw_scale), 2);
  mw_mesh_renderer* meshRenderer =
      mw_engine_get_field(it, sizeof(mw_mesh_renderer), 3);

  for (size_t i = 0; i < it->count; i++) {
    mat4 rotMat;
    glm_quat_mat4(rot[i], rotMat);

    mat4 transform;
    glm_mat4_identity(transform);
    glm_translate(transform, pos[i]);
    glm_mat4_mul(transform, rotMat, transform);

    glm_scale(transform, scale[i]);

    mw_camera* cam = mw_engine_get_component_it(it, ctx->main_camera_entity,
                                                MW_COMPONENT_CAMERA);

    ivec2 windowSize;
    mw_engine_get_window_size(ctx, windowSize);

    mw_render_pass rp = {.mesh_index = meshRenderer[i].mesh_id};

    mw_position* camPosition = mw_engine_get_component_it(
        it, ctx->main_camera_entity, MW_COMPONENT_POSITION);
    mw_rotation* camRotation = mw_engine_get_component_it(
        it, ctx->main_camera_entity, MW_COMPONENT_ROTATION);

    mat4 viewRotMat;
    glm_quat_mat4(*camRotation, viewRotMat);

    versor camRotInv;
    glm_quat_inv(*camRotation, camRotInv);
    glm_quat_mat4(camRotInv, rp.view);

    vec3 negCamPos;
    glm_vec3_negate_to(*camPosition, negCamPos);
    glm_translate(rp.view, negCamPos);

    glm_vec3_copy(*camPosition, rp.viewPos);

    glm_perspective(glm_rad(cam->fov), (float)windowSize[0] / windowSize[1],
                    cam->near_plane, cam->far_plane, rp.proj);

    glm_mat4_copy(transform, rp.transform);
    mw_renderer_push_render_pass(ctx->renderer, rp);
  }
}