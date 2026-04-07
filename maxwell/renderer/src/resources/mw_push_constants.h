#ifndef MW_PUSH_CONSTANTS_H
#define MW_PUSH_CONSTANTS_H

#include <cglm/cglm.h>

#include "mw_renderer.h"

typedef struct mw_push_constants_data {
  mat4 model;
  mat4 vp;
  vec4 viewPos;
} mw_push_constants_data;

mw_push_constants_data mw_renderer_get_push_constants(mw_renderer* renderer,
                                                      mat4 transform);

#endif