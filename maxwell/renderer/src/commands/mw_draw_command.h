#ifndef MW_DRAW_COMMAND_H
#define MW_DRAW_COMMAND_H

#include <vulkan/vulkan.h>

#include "core/mw_renderer_def.h"
#include "renderer/mw_pipeline.h"
#include "renderer/mw_swapchain.h"
#include "resources/mw_buffer.h"

void mw_renderer_record_draw_command_buffer(mw_renderer* renderer,
                                            mw_buffer vertexBuffer,
                                            mw_buffer indexBuffer,
                                            unsigned int imageIndex);

#endif