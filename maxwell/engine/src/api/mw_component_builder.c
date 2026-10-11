#include "api/mw_component_builder.h"

#include "ecs/mw_ecs.h"
#include "ecs/mw_transform.h"

mw_camera* mw_build_camera(mw_ctx* ctx, uint64_t entity, float near_plane,
                           float far_plane, float fov) {
  mw_camera* comp = mw_engine_entity_add_component(ctx, entity, MW_CAMERA);
  comp->near_plane = near_plane;
  comp->far_plane = far_plane;
  comp->fov = fov;
  return comp;
}

void mw_build_transform(mw_ctx* ctx, uint64_t entity) {
  mw_build_transform_no_scale(ctx, entity);
  mw_scale* scale = mw_engine_entity_add_component(ctx, entity, MW_SCALE);
  glm_vec3_one(*scale);
}

void mw_build_transform_no_scale(mw_ctx* ctx, uint64_t entity) {
  mw_position* pos = mw_engine_entity_add_component(ctx, entity, MW_POSITION);
  glm_vec3_zero(*pos);
  mw_rotation* rot = mw_engine_entity_add_component(ctx, entity, MW_ROTATION);
  glm_quat_identity(*rot);
}