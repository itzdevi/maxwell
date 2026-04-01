#ifndef MW_DESCRIPTOR_H
#define MW_DESCRIPTOR_H

#include <vulkan/vulkan.h>

#include "core/mw_renderer_def.h"

void mw_renderer_create_descriptor_set_layout(mw_renderer* renderer,
                                              uint32_t binding,
                                              VkDescriptorType descriptorType,
                                              VkShaderStageFlags shaderStage);
void mw_renderer_destroy_descriptor_set_layout(mw_renderer* renderer);

void mw_renderer_create_descriptor_pool(mw_renderer* renderer,
                                        VkDescriptorType descriptorType,
                                        uint32_t framesInFlight);
void mw_renderer_destroy_descriptor_pool(mw_renderer* renderer);

void mw_renderer_create_descriptor_sets(mw_renderer* renderer,
                                        uint32_t framesInFlight);
void mw_renderer_destroy_descriptor_sets(mw_renderer* renderer);

#endif