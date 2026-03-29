#ifndef MW_RENDERER_DEF_H
#define MW_RENDERER_DEF_H

#include <mw_gpu.h>
#include <vulkan/vulkan.h>

#include "renderer/mw_pipeline.h"
#include "renderer/mw_swapchain.h"
#include "resources/mw_uniform_buffer.h"

typedef struct mw_sync_objects {
  VkFence* frameFences;
  VkSemaphore* acquireSemaphores;
  VkSemaphore* submitSemaphores;
  uint8_t framesInFlight;
  uint8_t swapchainImages;
} mw_sync_objects;

typedef struct mw_renderer {
  mw_gpu_context gpu;
  VkSurfaceKHR surface;
  mw_swapchain swapchain;
  VkDescriptorSetLayout descriptorSetLayout;
  mw_pipeline pipeline;
  VkFramebuffer* framebuffers;
  mw_command drawCommand;
  mw_command transferCommand;
  mw_sync_objects syncObjects;
  int framebufferResized;
  vec2 framebufferSize;
  uint8_t currentFrame;
  mw_ubo* uniformBuffers;
} mw_renderer;

#endif