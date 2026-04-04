#include "mw_pipeline.h"

#include <mw_core.h>

#include "core/mw_renderer_def.h"
#include "mw_renderer_vertex.h"
#include "resources/mw_push_constants.h"

VkShaderModule mw_create_shader_module(VkDevice device, const char* code,
                                       unsigned int size) {
  VkShaderModuleCreateInfo createInfo = {0};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = size;
  createInfo.pCode = (unsigned int*)code;

  VkShaderModule shaderModule;
  MW_ASSERT(vkCreateShaderModule(device, &createInfo, NULL, &shaderModule) ==
            VK_SUCCESS);
  return shaderModule;
}

VkRenderPass mw_create_render_pass(VkDevice device,
                                   VkFormat swapchainImageFormat) {
  VkAttachmentDescription colorAttachment = {0};
  colorAttachment.format = swapchainImageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef = {0};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass = {0};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  VkRenderPassCreateInfo renderPassCreateInfo = {0};
  renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassCreateInfo.attachmentCount = 1;
  renderPassCreateInfo.pAttachments = &colorAttachment;
  renderPassCreateInfo.subpassCount = 1;
  renderPassCreateInfo.pSubpasses = &subpass;

  VkSubpassDependency dependency = {0};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  renderPassCreateInfo.dependencyCount = 1;
  renderPassCreateInfo.pDependencies = &dependency;

  VkRenderPass renderPass;
  MW_ASSERT(vkCreateRenderPass(device, &renderPassCreateInfo, NULL,
                               &renderPass) == VK_SUCCESS);

  return renderPass;
}

void mw_renderer_create_pipeline(mw_renderer* renderer) {
  mw_pipeline pipeline;
  pipeline.renderPass = mw_create_render_pass(renderer->gpu.device.device,
                                              renderer->swapchain.imageFormat);

  unsigned int vertSize = 0;
  unsigned int fragSize = 0;

  char* vertShaderCode = mw_read_file("../assets/shaders/vert.spv", &vertSize);
  char* fragShaderCode = mw_read_file("../assets/shaders/frag.spv", &fragSize);

  MW_ASSERT(vertSize % 4 == 0);
  MW_ASSERT(fragSize % 4 == 0);

  VkShaderModule vertShaderModule = mw_create_shader_module(
      renderer->gpu.device.device, vertShaderCode, vertSize);
  VkShaderModule fragShaderModule = mw_create_shader_module(
      renderer->gpu.device.device, fragShaderCode, fragSize);

  VkPipelineShaderStageCreateInfo vertStageCreateInfo = {0};
  vertStageCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertStageCreateInfo.module = vertShaderModule;
  vertStageCreateInfo.pName = "main";

  VkPipelineShaderStageCreateInfo fragStageCreateInfo = {0};
  fragStageCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragStageCreateInfo.module = fragShaderModule;
  fragStageCreateInfo.pName = "main";

  VkPipelineShaderStageCreateInfo shaderStages[] = {vertStageCreateInfo,
                                                    fragStageCreateInfo};

  VkVertexInputBindingDescription bindingDescription =
      mw_renderer_get_vertex_binding_description();
  VkVertexInputAttributeDescription attributeDescriptions[3] = {0};
  mw_renderer_get_vertex_attribute_description(attributeDescriptions);

  VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {0};
  vertexInputCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
  vertexInputCreateInfo.pVertexBindingDescriptions = &bindingDescription;
  vertexInputCreateInfo.vertexAttributeDescriptionCount = 3;
  vertexInputCreateInfo.pVertexAttributeDescriptions = attributeDescriptions;

  VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {0};
  inputAssemblyCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

  VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT,
                                    VK_DYNAMIC_STATE_SCISSOR};

  VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {0};
  dynamicStateCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicStateCreateInfo.dynamicStateCount =
      sizeof(dynamicStates) / sizeof(VkDynamicState);
  dynamicStateCreateInfo.pDynamicStates = dynamicStates;

  VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {0};
  viewportStateCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportStateCreateInfo.viewportCount = 1;
  viewportStateCreateInfo.scissorCount = 1;

  VkPipelineRasterizationStateCreateInfo rasterizer = {0};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1;
  rasterizer.cullMode = VK_CULL_MODE_NONE;
  rasterizer.depthBiasEnable = VK_FALSE;

  VkPipelineMultisampleStateCreateInfo multisampling = {0};
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  VkPipelineColorBlendAttachmentState colorBlendAttachment = {0};
  colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;
  // colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  // colorBlendAttachment.dstAlphaBlendFactor =
  //     VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  // colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
  // colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  // colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  // colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

  VkPipelineColorBlendStateCreateInfo colorBlending = {0};
  colorBlending.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;

  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {0};
  pipelineLayoutCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutCreateInfo.setLayoutCount = 1;
  pipelineLayoutCreateInfo.pSetLayouts = &renderer->descriptorSetLayout;

  VkPushConstantRange range = {0};
  range.offset = 0;
  range.size = sizeof(mw_push_constants_data);
  range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

  pipelineLayoutCreateInfo.pPushConstantRanges = &range;
  pipelineLayoutCreateInfo.pushConstantRangeCount = 1;

  MW_ASSERT(vkCreatePipelineLayout(renderer->gpu.device.device,
                                   &pipelineLayoutCreateInfo, NULL,
                                   &pipeline.layout) == VK_SUCCESS);

  VkGraphicsPipelineCreateInfo pipelineCreateInfo = {0};
  pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineCreateInfo.stageCount = 2;
  pipelineCreateInfo.pStages = shaderStages;

  pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
  pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
  pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
  pipelineCreateInfo.pRasterizationState = &rasterizer;
  pipelineCreateInfo.pMultisampleState = &multisampling;
  pipelineCreateInfo.pColorBlendState = &colorBlending;
  pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;

  pipelineCreateInfo.layout = pipeline.layout;
  pipelineCreateInfo.renderPass = pipeline.renderPass;
  pipelineCreateInfo.subpass = 0;

  MW_ASSERT(vkCreateGraphicsPipelines(renderer->gpu.device.device,
                                      VK_NULL_HANDLE, 1, &pipelineCreateInfo,
                                      NULL, &pipeline.pipeline) == VK_SUCCESS);

  free(vertShaderCode);
  free(fragShaderCode);

  vkDestroyShaderModule(renderer->gpu.device.device, vertShaderModule, NULL);
  vkDestroyShaderModule(renderer->gpu.device.device, fragShaderModule, NULL);

  renderer->pipeline = pipeline;
}

void mw_renderer_destroy_pipeline(mw_renderer* renderer) {
  vkDestroyPipeline(renderer->gpu.device.device, renderer->pipeline.pipeline,
                    NULL);
  vkDestroyPipelineLayout(renderer->gpu.device.device,
                          renderer->pipeline.layout, NULL);
  vkDestroyRenderPass(renderer->gpu.device.device,
                      renderer->pipeline.renderPass, NULL);
}