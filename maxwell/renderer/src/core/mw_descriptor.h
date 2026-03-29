#ifndef MW_DESCRIPTOR_H
#define MW_DESCRIPTOR_H

#include <vulkan/vulkan.h>

#include "core/mw_renderer_def.h"

void mw_renderer_create_descriptor_set_layout(mw_renderer* renderer,
                                              uint32_t binding,
                                              VkDescriptorType descriptorType,
                                              VkShaderStageFlags shaderStage);
void mw_renderer_destroy_descriptor_set_layout(mw_renderer* renderer);

#endif