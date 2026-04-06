#ifndef MW_APPLICATION_H
#define MW_APPLICATION_H

#include <cglm/cglm.h>
#include <vulkan/vulkan.h>

typedef void (*mw_app_callback)();

typedef struct mw_app_info {
  char* title;
  vec2 size;
  mw_app_callback onLoad;
} mw_app_info;

void mw_app_start(mw_app_info info);

#endif