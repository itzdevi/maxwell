#include "mw_push_constants.h"

#include "core/mw_renderer_def.h"

mw_push_constants_data mw_renderer_get_push_constants(mw_renderer* renderer,
                                                      mat4 transform) {
  float ar = renderer->framebuffer_size[0] / renderer->framebuffer_size[1];

  mat4 proj;
  glm_perspective(45, ar, 0.1, 1000, proj);

  vec4 viewPos = {0, 0, 3, 0};
  mat4 view;
  glm_mat4_identity(view);
  glm_lookat(viewPos, (vec3){}, (vec3){0, -1, 0}, view);

  mat4 mvp;
  glm_mul(proj, view, mvp);

  mw_push_constants_data data = {0};
  glm_mat4_copy(transform, data.model);
  glm_mat4_copy(mvp, data.vp);
  glm_vec4_copy(viewPos, data.view_pos);
  return data;
}