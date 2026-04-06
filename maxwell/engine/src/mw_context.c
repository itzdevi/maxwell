#include "mw_context.h"

#include "mw_engine_def.h"

mw_ctx* mw_engine_init(mw_engine_info info) {
  // TODO: create ECS scene
  mw_app_info appInfo = {.title = "Maxwell", .size = {800, 600}};
  mw_app_start(appInfo);
}