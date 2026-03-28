#ifndef MW_PIPELINE_H
#define MW_PIPELINE_H

#include <mw_gpu.h>
#include <vulkan/vulkan.h>

#include "mw_swapchain.h"

typedef struct mw_pipeline {
  VkPipeline pipeline;
  VkPipelineLayout layout;
  VkRenderPass renderPass;
} mw_pipeline;

mw_pipeline mw_renderer_create_pipeline(mw_device device,
                                        mw_swapchain swapchain);
void mw_renderer_destroy_pipeline(mw_device device, mw_pipeline pipeline);

#endif