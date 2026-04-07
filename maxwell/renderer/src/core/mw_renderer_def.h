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
  mw_command draw_command;
  mw_command transfer_command;
  mw_sync_objects sync_objects;
  int framebuffer_resized;
  vec2 framebuffer_size;
  uint8_t current_frame;
  mw_render_page* render_pages;
  uint32_t render_page_count;
  uint32_t current_push_page;
  uint32_t current_pop_page;
  uint32_t current_page_offset;
  mw_mesh* meshes;
  mw_mesh_entry* mesh_pool;
  uint32_t mesh_count;
  mw_buffer vertex_buffer;
  mw_buffer index_buffer;
} mw_renderer;

#endif