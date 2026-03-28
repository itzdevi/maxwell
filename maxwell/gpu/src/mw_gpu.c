#include "mw_gpu.h"

#include "mw_device.h"
#include "mw_instance.h"
#include "mw_physical_device.h"

mw_gpu_context mw_gpu_init(mw_gpu_config config, VkInstance instance) {
  mw_gpu_context ctx = {0};
  ctx.instance = instance;
  ctx.physicalDevice =
      mw_gpu_pick_physical_device(ctx.instance, config.surface);
  ctx.device = mw_gpu_create_device(ctx.physicalDevice, config.surface);

  return ctx;
}

void mw_gpu_destroy(mw_gpu_context ctx) {
  mw_gpu_destroy_device(ctx.device);
  mw_gpu_destroy_instance(ctx.instance);
}