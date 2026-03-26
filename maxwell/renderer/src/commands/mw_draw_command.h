#ifndef MW_DRAW_COMMAND_H
#define MW_DRAW_COMMAND_H

#include <vulkan/vulkan.h>

#include "core/mw_device.h"
#include "mw_command.h"
#include "renderer/mw_pipeline.h"
#include "renderer/mw_swapchain.h"
#include "resources/mw_buffer.h"

void mw_renderer_record_draw_command_buffer(
    mw_swapchain swapchain, mw_pipeline pipeline, mw_command commandBuffer,
    VkFramebuffer* framebuffers, mw_buffer vertexBuffer, mw_buffer indexBuffer,
    unsigned int imageIndex, unsigned int frameIndex);

#endif