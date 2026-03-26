#include "mw_renderer.h"

#include <vulkan/vulkan.h>

#include "mw_assert.h"
#include "mw_buffer.h"
#include "mw_command_buffer.h"
#include "mw_device.h"
#include "mw_framebuffer.h"
#include "mw_instance.h"
#include "mw_physical_device.h"
#include "mw_pipeline.h"
#include "mw_surface.h"
#include "mw_swapchain.h"
#include "mw_sync_objects.h"

const unsigned int MAX_FRAMES_IN_FLIGHT = 2;

VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice physicalDevice;
mw_device device;
mw_swapchain swapchain;
mw_pipeline pipeline;
VkFramebuffer* framebuffers;
mw_command_buffers commandBuffers;
mw_sync_objects syncObjects;
mw_buffer vertexBuffer;
int framebufferResized = 0;

void mw_recreate_swapchain(mw_device device, mw_swapchain* swapchain,
                           VkFramebuffer** framebuffers,
                           VkPhysicalDevice physicalDevice,
                           VkSurfaceKHR surface, mw_pipeline* pipeline,
                           mw_command_buffers* commandBuffers) {
  mw_vec2 size = mw_app_get_framebuffer_size();
  while (size.x == 0 || size.y == 0) {
    size = mw_app_get_framebuffer_size();
    mw_app_wait_events();
  }

  vkDeviceWaitIdle(device.device);

  mw_renderer_destroy_command_buffers(device, *commandBuffers);
  mw_renderer_destroy_sync_objects(device, syncObjects);
  mw_renderer_destroy_framebuffers(device, *swapchain, *framebuffers);
  mw_renderer_destroy_pipeline(device, *pipeline);
  mw_renderer_destroy_swapchain(device, *swapchain);

  *swapchain = mw_renderer_create_swapchain(physicalDevice, surface, device);
  *pipeline = mw_renderer_create_pipeline(device, *swapchain);
  *framebuffers =
      mw_renderer_create_framebuffers(device, *swapchain, *pipeline);
  *commandBuffers = mw_renderer_create_command_buffers(
      device, physicalDevice, surface, MAX_FRAMES_IN_FLIGHT);
  syncObjects = mw_renderer_create_sync_objects(device, MAX_FRAMES_IN_FLIGHT,
                                                swapchain->imageCount);
}

void mw_renderer_initialize(mw_app_info info, mw_vertex* vertices,
                            uint32_t verticesSize) {
  instance = mw_renderer_create_instance(info.title);
  surface = mw_renderer_create_surface(instance);
  physicalDevice = mw_renderer_pick_physical_device(instance, surface);
  device = mw_renderer_create_device(physicalDevice, surface);
  swapchain = mw_renderer_create_swapchain(physicalDevice, surface, device);

  pipeline = mw_renderer_create_pipeline(device, swapchain);
  framebuffers = mw_renderer_create_framebuffers(device, swapchain, pipeline);
  vertexBuffer = mw_renderer_create_buffer(device, physicalDevice, verticesSize,
                                           sizeof(mw_vertex));
  commandBuffers = mw_renderer_create_command_buffers(
      device, physicalDevice, surface, MAX_FRAMES_IN_FLIGHT);
  syncObjects = mw_renderer_create_sync_objects(device, MAX_FRAMES_IN_FLIGHT,
                                                swapchain.imageCount);

  mw_renderer_copy_memory(vertexBuffer, vertices);
}

void mw_renderer_cleanup() {
  vkDeviceWaitIdle(device.device);

  mw_renderer_destroy_sync_objects(device, syncObjects);
  mw_renderer_destroy_command_buffers(device, commandBuffers);
  mw_renderer_destroy_buffer(device, vertexBuffer);
  mw_renderer_destroy_framebuffers(device, swapchain, framebuffers);
  mw_renderer_destroy_pipeline(device, pipeline);
  mw_renderer_destroy_swapchain(device, swapchain);

  mw_renderer_destroy_device(device);
  mw_renderer_destroy_surface(instance, surface);
  mw_renderer_destroy_instance(instance);
}

unsigned int currentFrame = 0;
void mw_renderer_draw_frame() {
  VkFence frameFence = syncObjects.frameFences[currentFrame];
  vkWaitForFences(device.device, 1, &frameFence, VK_TRUE, UINT64_MAX);

  unsigned int imageIndex;
  VkSemaphore acquireSemaphore = syncObjects.acquireSemaphores[currentFrame];
  VkResult result =
      vkAcquireNextImageKHR(device.device, swapchain.swapchain, UINT64_MAX,
                            acquireSemaphore, VK_NULL_HANDLE, &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    mw_recreate_swapchain(device, &swapchain, &framebuffers, physicalDevice,
                          surface, &pipeline, &commandBuffers);
    return;
  } else
    MW_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR);

  vkResetFences(device.device, 1, &frameFence);

  VkSemaphore submitSemaphore = syncObjects.submitSemaphores[imageIndex];

  vkResetCommandBuffer(commandBuffers.buffers[currentFrame], 0);
  mw_renderer_record_command_buffer(swapchain, pipeline, commandBuffers,
                                    framebuffers, vertexBuffer, imageIndex,
                                    currentFrame);

  VkSubmitInfo submitInfo = {0};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffers.buffers[currentFrame];
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = &acquireSemaphore;
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = &submitSemaphore;

  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.pWaitDstStageMask = waitStages;

  MW_ASSERT(vkQueueSubmit(device.graphicsQueue, 1, &submitInfo, frameFence) ==
            VK_SUCCESS);

  VkPresentInfoKHR presentInfo = {0};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = &submitSemaphore;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &swapchain.swapchain;
  presentInfo.pImageIndices = &imageIndex;

  result = vkQueuePresentKHR(device.presentQueue, &presentInfo);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      framebufferResized) {
    framebufferResized = 0;
    mw_recreate_swapchain(device, &swapchain, &framebuffers, physicalDevice,
                          surface, &pipeline, &commandBuffers);
  } else
    MW_ASSERT(result == VK_SUCCESS);

  currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void mw_renderer_wait_for_idle() { vkDeviceWaitIdle(device.device); }

void mw_renderer_update_framebuffer_resized() { framebufferResized = 1; }