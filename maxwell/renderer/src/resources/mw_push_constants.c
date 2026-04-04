#include "mw_push_constants.h"

#include "core/mw_renderer_def.h"

mw_push_constants_data mw_renderer_get_push_constants(mw_renderer* renderer,
                                                      mat4 transform) {
  float ar = renderer->framebufferSize[0] / renderer->framebufferSize[1];

  mat4 proj;
  glm_perspective(45, ar, 0.1, 1000, proj);

  mat4 view;
  glm_mat4_identity(view);
  glm_lookat((vec3){0, 0, 3}, (vec3){}, (vec3){0, -1, 0}, view);

  mat4 mvp;
  glm_mul(proj, view, mvp);
  glm_mul(mvp, transform, mvp);

  mw_push_constants_data data = {0};
  glm_mat4_copy(mvp, data.mvp);
  return data;
}