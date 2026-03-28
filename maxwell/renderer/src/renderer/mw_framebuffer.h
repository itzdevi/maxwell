#ifndef MW_FRAMEBUFFER_H
#define MW_FRAMEBUFFER_H

#include <mw_gpu.h>
#include <vulkan/vulkan.h>

#include "mw_pipeline.h"
#include "mw_swapchain.h"

VkFramebuffer* mw_renderer_create_framebuffers(mw_device device,
                                               mw_swapchain swapchain,
                                               mw_pipeline pipeline);
void mw_renderer_destroy_framebuffers(mw_device device, mw_swapchain swapchain,
                                      VkFramebuffer* framebuffers);

#endif