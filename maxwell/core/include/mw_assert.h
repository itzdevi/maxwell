#ifndef MW_ASSERT_H
#define MW_ASSERT_H

#include <stdio.h>
#include <stdlib.h>

#define MW_ASSERT(cond)                                                   \
  do {                                                                    \
    if (!(cond)) {                                                        \
      fprintf(stderr, "Assertion failed at %s:%d\n", __FILE__, __LINE__); \
      exit(EXIT_FAILURE);                                                 \
    }                                                                     \
  } while (0)

#endif