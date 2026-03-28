#ifndef MW_MACROS_H
#define MW_MACROS_H

#include <assert.h>

static_assert(sizeof(float) == 4);
static_assert(sizeof(double) == 8);

#define f32 float
#define f64 double

#endif