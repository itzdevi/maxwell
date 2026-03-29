#include "mw_descriptor.h"

#include <mw_core.h>
#include <vulkan/vulkan.h>

void mw_renderer_create_descriptor_set_layout(mw_renderer* renderer,
                                              uint32_t binding,
                                              VkDescriptorType descriptorType,
                                              VkShaderStageFlags shaderStage) {
  VkDescriptorSetLayoutBinding descriptorLayoutBinding = {0};
  descriptorLayoutBinding.binding = binding;
  descriptorLayoutBinding.descriptorType = descriptorType;
  descriptorLayoutBinding.descriptorCount = 1;
  descriptorLayoutBinding.stageFlags = shaderStage;

  VkDescriptorSetLayoutCreateInfo layoutInfo = {0};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = 1;
  layoutInfo.pBindings = &descriptorLayoutBinding;

  MW_ASSERT(vkCreateDescriptorSetLayout(
                renderer->gpu.device.device, &layoutInfo, NULL,
                &renderer->descriptorSetLayout) == VK_SUCCESS);
}

void mw_renderer_destroy_descriptor_set_layout(mw_renderer* renderer) {
  vkDestroyDescriptorSetLayout(renderer->gpu.device.device,
                               renderer->descriptorSetLayout, NULL);
}