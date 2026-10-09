#include "mw_push_constants.h"

#include "core/mw_renderer_def.h"

mw_push_constants_data mw_renderer_get_push_constants(mw_renderer* renderer,
                                                      mat4 transform, mat4 view,
                                                      mat4 proj, vec3 viewPos) {
  mat4 vp;
  glm_mul(proj, view, vp);

  mw_push_constants_data data = {0};
  glm_mat4_copy(transform, data.model);
  glm_mat4_copy(vp, data.vp);
  glm_vec3_copy(viewPos, data.view_pos);
  data.view_pos[3] = 0;
  return data;
}