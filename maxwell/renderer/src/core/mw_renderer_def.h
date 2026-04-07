#ifndef MW_RENDERER_DEF_H
#define MW_RENDERER_DEF_H

#include <mw_gpu.h>
#include <vulkan/vulkan.h>

#include "renderer/mw_pipeline.h"
#include "renderer/mw_render_queue.h"
#include "renderer/mw_swapchain.h"
#include "renderer/mw_sync_objects.h"
#include "resources/mw_buffer.h"
#include "resources/mw_mesh.h"

typedef struct mw_renderer {
  mw_gpu_context gpu;
  VkSurfaceKHR surface;
  mw_swapchain swapchain;
  mw_pipeline pipeline;
  VkFramebuffer* framebuffers;
  mw_command drawCommand;
  mw_command transferCommand;
  mw_sync_objects syncObjects;
  int framebufferResized;
  vec2 framebufferSize;
  uint8_t currentFrame;
  mw_render_page* renderPages;
  uint32_t renderPagesCount;
  uint32_t currentPushPage;
  uint32_t currentPopPage;
  uint32_t currentPageOffset;
  mw_mesh* meshes;
  mw_mesh_entry* meshPool;
  uint32_t meshCount;
  mw_buffer vertexBuffer;
  mw_buffer indexBuffer;
} mw_renderer;

#endif