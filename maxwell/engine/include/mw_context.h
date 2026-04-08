#ifndef MW_CONTEXT_H
#define MW_CONTEXT_H

#include <cglm/cglm.h>

typedef struct mw_ctx mw_ctx;

typedef struct mw_engine_info {
  char* title;
  vec2 size;
} mw_engine_info;

mw_ctx* mw_engine_create(mw_engine_info info);
void mw_engine_run(mw_ctx* ctx);
void mw_engine_destroy(mw_ctx* ctx);

#endif