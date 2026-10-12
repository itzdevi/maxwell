#ifndef MW_TIME_H
#define MW_TIME_H

#include "ecs/mw_ecs.h"

typedef struct mw_time {
  double time;
  double delta_time;
} mw_time;

mw_time mw_get_time(mw_iter* it);

#endif