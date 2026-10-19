#include "ecs/mw_velocity_solver.h"

#include "api/mw_time.h"
#include "ecs/mw_transform.h"

void mw_velocity_solver(it* it) {
  mw_position* position = mw_engine_get_field(it, sizeof(mw_position), 0);
  mw_velocity* velocity = mw_engine_get_field(it, sizeof(mw_velocity), 1);

  mw_time time = mw_get_time_it(it);

  for (size_t i = 0; i < mw_engine_get_iter_count(it); i++) {
    vec3 velocityScaleTime;
    glm_vec3_scale(velocity[i], time.delta_time, velocityScaleTime);
    glm_vec3_add(position[i], velocityScaleTime, position[i]);
  }
}