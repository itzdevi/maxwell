#include "mw_renderer.h"

#include <vulkan/vulkan.h>

#include "mw_assert.h"
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

void mw_renderer_initialize(mw_app_info info) {
  instance = mw_renderer_create_instance(info.title);
  surface = mw_renderer_create_surface(instance);
  physicalDevice = mw_renderer_pick_physical_device(instance, surface);
  device = mw_renderer_create_device(physicalDevice, surface);
  swapchain = mw_renderer_create_swapchain(physicalDevice, surface, device);
  pipeline = mw_renderer_create_pipeline(device, swapchain);
  framebuffers = mw_renderer_create_framebuffers(device, swapchain, pipeline);
  commandBuffers = mw_renderer_create_command_buffers(
      device, physicalDevice, surface, MAX_FRAMES_IN_FLIGHT);
  syncObjects = mw_renderer_create_sync_objects(device, MAX_FRAMES_IN_FLIGHT);
}

void mw_renderer_cleanup() {
  mw_renderer_destroy_sync_objects(device, syncObjects);
  mw_renderer_destroy_command_buffers(device, commandBuffers);
  mw_renderer_destroy_framebuffers(device, swapchain, framebuffers);
  mw_renderer_destroy_pipeline(device, pipeline);
  mw_renderer_destroy_swapchain(device, swapchain);
  mw_renderer_destroy_instance(instance);
  mw_renderer_destroy_device(device);
  mw_renderer_destroy_surface(instance, surface);
}

unsigned int currentFrame;

void mw_renderer_draw_frame() {
  vkWaitForFences(device.device, 1, &syncObjects.inFlightFences[currentFrame],
                  VK_TRUE, UINT64_MAX);
  vkResetFences(device.device, 1, &syncObjects.inFlightFences[currentFrame]);

  unsigned int imageIndex;
  vkAcquireNextImageKHR(device.device, swapchain.swapchain, UINT64_MAX,
                        syncObjects.imageAvailableSemaphores[currentFrame],
                        VK_NULL_HANDLE, &imageIndex);

  vkResetCommandBuffer(commandBuffers.buffers[currentFrame], 0);
  mw_renderer_record_command_buffer(swapchain, pipeline, commandBuffers,
                                    framebuffers, imageIndex, currentFrame);

  VkSubmitInfo submitInfo = {0};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {
      syncObjects.imageAvailableSemaphores[currentFrame]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffers.buffers[currentFrame];

  VkSemaphore signalSemaphores[] = {
      syncObjects.renderFinishedSemaphores[currentFrame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  MW_ASSERT(vkQueueSubmit(device.graphicsQueue, 1, &submitInfo,
                          syncObjects.inFlightFences[currentFrame]) ==
            VK_SUCCESS);

  VkPresentInfoKHR presentInfo = {0};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapchains[] = {swapchain.swapchain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapchains;
  presentInfo.pImageIndices = &imageIndex;

  vkQueuePresentKHR(device.presentQueue, &presentInfo);

  currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void mw_renderer_wait_for_idle() { vkDeviceWaitIdle(device.device); }