#ifndef MW_CONTEXT_H
#define MW_CONTEXT_H

typedef struct mw_ctx mw_ctx;

typedef struct mw_engine_info {
  char* title;
} mw_engine_info;

mw_ctx* mw_engine_init(mw_engine_info info);

#endif