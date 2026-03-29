#include "mw_uniform_buffer.h"

#include <string.h>

#include "core/mw_renderer_def.h"

void mw_renderer_create_uniform_buffers(mw_renderer* renderer,
                                        uint32_t framesInFlight) {
  VkDeviceSize bufferSize = sizeof(mw_mvp);
  renderer->uniformBuffers = malloc(sizeof(mw_ubo) * framesInFlight);

  for (size_t i = 0; i < framesInFlight; i++) {
    renderer->uniformBuffers[i].buffer = mw_renderer_create_buffer(
        renderer->gpu.device, renderer->gpu.physicalDevice, renderer->surface,
        framesInFlight, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkMapMemory(renderer->gpu.device.device,
                renderer->uniformBuffers[i].buffer.memory, 0, bufferSize, 0,
                &renderer->uniformBuffers[i].data);
  }
}

void mw_renderer_destroy_uniform_buffers(mw_renderer* renderer,
                                         uint32_t framesInFlight) {
  for (size_t i = 0; i < framesInFlight; i++) {
    vkUnmapMemory(renderer->gpu.device.device,
                  renderer->uniformBuffers[i].buffer.memory);
    mw_renderer_destroy_buffer(renderer->gpu.device,
                               renderer->uniformBuffers[i].buffer);
  }
  free(renderer->uniformBuffers);
}

void mw_renderer_update_current_uniform_buffer(mw_renderer* renderer) {
  float dt = 0.01f;
  mw_mvp mvp = {0};
  glm_mat4_identity(mvp.model);
  glm_perspective_default((float)800 / 600, mvp.proj);
  glm_lookat((vec3){3, 0, 0}, (vec3){0, 0, 0}, (vec3){0, 0, 1}, mvp.view);
  memcpy(renderer->uniformBuffers[renderer->currentFrame].data, &mvp,
         sizeof(mw_mvp));
}