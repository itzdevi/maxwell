#include "mw_math_func.h"

#include <sys/param.h>

int mw_clamp(int x, int lo, int hi) { return MIN(hi, MAX(lo, x)); }