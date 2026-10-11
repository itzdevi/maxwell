#ifndef MW_COMPONENT_BUILDER_H
#define MW_COMPONENT_BUILDER_H

#include "ecs/mw_camera.h"
#include "mw_context.h"

mw_camera* mw_build_camera(mw_ctx* ctx, uint64_t entity, float near_plane,
                           float far_plane, float fov);
void mw_build_transform(mw_ctx* ctx, uint64_t entity);
void mw_build_transform_no_scale(mw_ctx* ctx, uint64_t entity);

#endif