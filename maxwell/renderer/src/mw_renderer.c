#include "mw_renderer.h"

#include <mw_core.h>
#include <mw_gpu.h>
#include <string.h>
#include <vulkan/vulkan.h>

#include "commands/mw_draw_command.h"
#include "core/mw_descriptor.h"
#include "core/mw_renderer_def.h"
#include "core/mw_surface.h"
#include "renderer/mw_framebuffer.h"
#include "renderer/mw_pipeline.h"
#include "renderer/mw_render_queue.h"
#include "renderer/mw_swapchain.h"
#include "renderer/mw_sync_objects.h"
#include "resources/mw_buffer.h"
#include "resources/mw_index_buffer.h"
#include "resources/mw_vertex_buffer.h"

const unsigned int MAX_FRAMES_IN_FLIGHT = 2;

void mw_recreate_swapchain(mw_renderer* renderer) {
  vkDeviceWaitIdle(renderer->gpu.device.device);

  mw_gpu_destroy_command(renderer->gpu.device, renderer->drawCommand);
  mw_renderer_destroy_sync_objects(renderer->gpu.device, renderer->syncObjects);
  mw_renderer_destroy_framebuffers(renderer->gpu.device, renderer->swapchain,
                                   renderer->framebuffers);
  mw_renderer_destroy_pipeline(renderer);

  mw_renderer_create_swapchain(renderer);
  mw_renderer_create_pipeline(renderer);
  mw_renderer_create_framebuffers(renderer);
  renderer->drawCommand =
      mw_gpu_create_command(renderer->gpu.device, renderer->gpu.physicalDevice,
                            renderer->surface, MAX_FRAMES_IN_FLIGHT, GRAPHICS);
  renderer->syncObjects = mw_renderer_create_sync_objects(
      renderer->gpu.device, MAX_FRAMES_IN_FLIGHT,
      renderer->swapchain.imageCount);
}

mw_renderer* mw_renderer_initialize(mw_renderer_config config) {
  mw_renderer* renderer = calloc(1, sizeof(mw_renderer));
  renderer->framebufferResized = 0;
  renderer->currentFrame = 0;

  glm_vec2_copy(config.framebufferSize, renderer->framebufferSize);

  mw_gpu_config gpuConfig = {.title = config.title,
                             .ext = config.extensions,
                             .extCount = config.extensionCount};
  VkInstance instance = mw_gpu_create_instance(gpuConfig);
  renderer->surface = mw_renderer_create_surface(config, instance);
  gpuConfig.surface = renderer->surface;
  renderer->gpu = mw_gpu_init(gpuConfig, instance);

  mw_renderer_create_swapchain(renderer);

  mw_renderer_create_uniform_buffers(renderer, MAX_FRAMES_IN_FLIGHT);

  mw_renderer_create_descriptor_set_layout(renderer, 0,
                                           VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                           VK_SHADER_STAGE_VERTEX_BIT);
  mw_renderer_create_descriptor_pool(
      renderer, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT);
  mw_renderer_create_descriptor_sets(renderer, MAX_FRAMES_IN_FLIGHT);

  mw_renderer_create_pipeline(renderer);
  mw_renderer_create_framebuffers(renderer);
  renderer->transferCommand =
      mw_gpu_create_command(renderer->gpu.device, renderer->gpu.physicalDevice,
                            renderer->surface, 1, TRANSFER);
  renderer->syncObjects = mw_renderer_create_sync_objects(
      renderer->gpu.device, MAX_FRAMES_IN_FLIGHT,
      renderer->swapchain.imageCount);

  renderer->drawCommand =
      mw_gpu_create_command(renderer->gpu.device, renderer->gpu.physicalDevice,
                            renderer->surface, MAX_FRAMES_IN_FLIGHT, GRAPHICS);

  mw_renderer_create_render_page(renderer);
  mw_renderer_clean_render_queue(renderer);

  return renderer;
}

void mw_renderer_destroy(mw_renderer* renderer) {
  vkDeviceWaitIdle(renderer->gpu.device.device);

  mw_renderer_destroy_render_pages(renderer);

  mw_renderer_destroy_sync_objects(renderer->gpu.device, renderer->syncObjects);
  mw_gpu_destroy_command(renderer->gpu.device, renderer->drawCommand);
  mw_gpu_destroy_command(renderer->gpu.device, renderer->transferCommand);
  mw_renderer_destroy_framebuffers(renderer->gpu.device, renderer->swapchain,
                                   renderer->framebuffers);
  mw_renderer_destroy_pipeline(renderer);
  mw_renderer_destroy_swapchain(renderer);
  mw_renderer_destroy_uniform_buffers(renderer, MAX_FRAMES_IN_FLIGHT);
  mw_renderer_destroy_descriptor_pool(renderer);
  mw_renderer_destroy_descriptor_set_layout(renderer);
  mw_renderer_destroy_descriptor_sets(renderer);

  mw_renderer_destroy_surface(renderer->gpu.instance, renderer->surface);

  mw_gpu_destroy(renderer->gpu);

  free(renderer);
}

void mw_renderer_draw_frame(mw_renderer* renderer) {
  VkFence frameFence =
      renderer->syncObjects.frameFences[renderer->currentFrame];
  vkWaitForFences(renderer->gpu.device.device, 1, &frameFence, VK_TRUE,
                  UINT64_MAX);

  unsigned int imageIndex;
  VkSemaphore acquireSemaphore =
      renderer->syncObjects.acquireSemaphores[renderer->currentFrame];
  VkResult result = vkAcquireNextImageKHR(
      renderer->gpu.device.device, renderer->swapchain.swapchain, UINT64_MAX,
      acquireSemaphore, VK_NULL_HANDLE, &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    mw_recreate_swapchain(renderer);
    return;
  } else
    MW_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR);

  vkResetFences(renderer->gpu.device.device, 1, &frameFence);

  VkSemaphore submitSemaphore =
      renderer->syncObjects.submitSemaphores[imageIndex];

  // Update uniform buffer BEFORE recording command buffer
  mw_renderer_update_current_uniform_buffer(renderer);

  vkResetCommandBuffer(renderer->drawCommand.buffers[renderer->currentFrame],
                       0);
  mw_renderer_record_draw_command_buffer(renderer, imageIndex);

  VkSubmitInfo submitInfo = {0};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers =
      &renderer->drawCommand.buffers[renderer->currentFrame];
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = &acquireSemaphore;
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = &submitSemaphore;

  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.pWaitDstStageMask = waitStages;

  MW_ASSERT(vkQueueSubmit(renderer->gpu.device.graphicsQueue, 1, &submitInfo,
                          frameFence) == VK_SUCCESS);

  VkPresentInfoKHR presentInfo = {0};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = &submitSemaphore;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &renderer->swapchain.swapchain;
  presentInfo.pImageIndices = &imageIndex;

  result = vkQueuePresentKHR(renderer->gpu.device.presentQueue, &presentInfo);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      renderer->framebufferResized) {
    renderer->framebufferResized = 0;
  } else {
    if (!renderer->framebufferResized) {
      mw_recreate_swapchain(renderer);
    }
    MW_ASSERT(result == VK_SUCCESS);
  }

  renderer->currentFrame = (renderer->currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
  mw_renderer_clean_render_queue(renderer);
}

void mw_renderer_push_render_queue(mw_renderer* renderer, uint32_t meshIndex) {}

void mw_renderer_wait_for_idle(mw_renderer* renderer) {
  vkDeviceWaitIdle(renderer->gpu.device.device);
}

void mw_renderer_set_framebuffer_size(mw_renderer* renderer, vec2 size) {
  glm_vec2_copy(size, renderer->framebufferSize);
  renderer->framebufferResized = 1;
}