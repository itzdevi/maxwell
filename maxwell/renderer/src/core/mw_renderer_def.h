#ifndef MW_RENDERER_DEF_H
#define MW_RENDERER_DEF_H

#include <mw_gpu.h>
#include <vulkan/vulkan.h>

#include "renderer/mw_pipeline.h"
#include "renderer/mw_render_queue.h"
#include "renderer/mw_swapchain.h"
#include "renderer/mw_sync_objects.h"
#include "resources/mw_uniform_buffer.h"

typedef struct mw_renderer {
  mw_gpu_context gpu;
  VkSurfaceKHR surface;
  mw_swapchain swapchain;
  VkDescriptorSetLayout descriptorSetLayout;
  VkDescriptorPool descriptorPool;
  VkDescriptorSet* descriptorSets;
  mw_pipeline pipeline;
  VkFramebuffer* framebuffers;
  mw_command drawCommand;
  mw_command transferCommand;
  mw_sync_objects syncObjects;
  int framebufferResized;
  vec2 framebufferSize;
  uint8_t currentFrame;
  mw_buffer meshBuffer;
  mw_ubo* uniformBuffers;
  mw_render_page* renderPages;
  uint32_t renderPagesCount;
  uint32_t currentPage;
  uint32_t currentPageOffset;
} mw_renderer;

#endif