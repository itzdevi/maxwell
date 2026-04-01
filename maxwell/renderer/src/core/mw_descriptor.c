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

void mw_renderer_create_descriptor_pool(mw_renderer* renderer,
                                        VkDescriptorType descriptorType,
                                        uint32_t framesInFlight) {
  VkDescriptorPoolSize poolSize = {0};
  poolSize.type = descriptorType;
  poolSize.descriptorCount = framesInFlight;

  VkDescriptorPoolCreateInfo poolInfo = {0};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = 1;
  poolInfo.pPoolSizes = &poolSize;
  poolInfo.maxSets = framesInFlight;

  MW_ASSERT(vkCreateDescriptorPool(renderer->gpu.device.device, &poolInfo, NULL,
                                   &renderer->descriptorPool) == VK_SUCCESS);
}

void mw_renderer_destroy_descriptor_pool(mw_renderer* renderer) {
  vkDestroyDescriptorPool(renderer->gpu.device.device, renderer->descriptorPool,
                          NULL);
}

void mw_renderer_create_descriptor_sets(mw_renderer* renderer,
                                        uint32_t framesInFlight) {
  // Create array of layout pointers for allocation
  VkDescriptorSetLayout* layouts =
      malloc(framesInFlight * sizeof(VkDescriptorSetLayout));
  for (size_t i = 0; i < framesInFlight; i++) {
    layouts[i] = renderer->descriptorSetLayout;
  }

  VkDescriptorSetAllocateInfo allocInfo = {0};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = renderer->descriptorPool;
  allocInfo.descriptorSetCount = framesInFlight;
  allocInfo.pSetLayouts = layouts;

  renderer->descriptorSets = malloc(framesInFlight * sizeof(VkDescriptorSet));
  MW_ASSERT(vkAllocateDescriptorSets(renderer->gpu.device.device, &allocInfo,
                                     renderer->descriptorSets) == VK_SUCCESS);

  for (size_t i = 0; i < framesInFlight; i++) {
    VkDescriptorBufferInfo bufferInfo = {0};
    bufferInfo.buffer = renderer->uniformBuffers[i].buffer.buffer;
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(mw_mvp);

    VkWriteDescriptorSet descriptorWrite = {0};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = renderer->descriptorSets[i];
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(renderer->gpu.device.device, 1, &descriptorWrite, 0,
                           NULL);
  }

  free(layouts);
}

void mw_renderer_destroy_descriptor_sets(mw_renderer* renderer) {
  free(renderer->descriptorSets);
}