#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "assert.h"

char* readFile(const char* filename, unsigned int* sizeOut) {
  FILE* file;
  unsigned char* buffer;
  unsigned int fileSize = 0;
  size_t readSize = 0;

  file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "Failed to open file at path \"%s\"!\n", filename);
    return NULL;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    fprintf(stderr, "Failed to seek end of file at path \"%s\"!\n", filename);
    fclose(file);
    return NULL;
  }
  fileSize = ftell(file);
  if (fileSize == -1) {
    fprintf(stderr, "Failed to get file position at path \"%s\"!\n", filename);
    fclose(file);
    return NULL;
  }
  rewind(file);

  buffer = malloc(fileSize + 1);
  if (!buffer) {
    fprintf(stderr, "Failed to allocate memory buffer!\n");
    fclose(file);
    return NULL;
  }

  readSize = fread(buffer, 1, fileSize, file);
  if (readSize != fileSize) {
    fprintf(stderr, "Failed to read file content at path \"%s\"!\n", filename);
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[fileSize] = '\0';

  fclose(file);
  if (sizeOut != NULL) *sizeOut = fileSize;

  return buffer;
}

VkRenderPass renderPass;
VkPipelineLayout pipelineLayout;
VkPipeline graphicsPipeline;
VkFramebuffer* swapchainFramebuffers;
VkCommandPool commandPool;
VkCommandBuffer commandBuffer;
VkSemaphore imageAvailableSemaphore;
VkSemaphore renderFinishedSemaphore;
VkFence inFlightFence;

unsigned int swapchainImageCount;

VkShaderModule createShaderModule(const char* code, unsigned int size) {
  VkShaderModuleCreateInfo createInfo = {0};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = size;
  createInfo.pCode = (unsigned int*)code;

  VkShaderModule shaderModule;
  MW_ASSERT(vkCreateShaderModule(device, &createInfo, NULL, &shaderModule) ==
            VK_SUCCESS);
  return shaderModule;
}

void createGraphicsPipeline() {
  unsigned int vertSize = 0;
  unsigned int fragSize = 0;

  char* vertShaderCode = readFile("../shaders/vert.spv", &vertSize);
  char* fragShaderCode = readFile("../shaders/frag.spv", &fragSize);

  MW_ASSERT(vertSize % 4 == 0);
  MW_ASSERT(fragSize % 4 == 0);

  VkShaderModule vertShaderModule =
      createShaderModule(vertShaderCode, vertSize);
  VkShaderModule fragShaderModule =
      createShaderModule(fragShaderCode, fragSize);

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

  VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {0};
  vertexInputCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
  vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;

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

  MW_ASSERT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL,
                                   &pipelineLayout) == VK_SUCCESS);

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

  pipelineCreateInfo.layout = pipelineLayout;
  pipelineCreateInfo.renderPass = renderPass;
  pipelineCreateInfo.subpass = 0;

  MW_ASSERT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1,
                                      &pipelineCreateInfo, NULL,
                                      &graphicsPipeline) == VK_SUCCESS);

  free(vertShaderCode);
  free(fragShaderCode);

  vkDestroyShaderModule(device, vertShaderModule, NULL);
  vkDestroyShaderModule(device, fragShaderModule, NULL);
}

void createRenderPass() {
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

  MW_ASSERT(vkCreateRenderPass(device, &renderPassCreateInfo, NULL,
                               &renderPass) == VK_SUCCESS);
}

void createFramebuffers() {
  swapchainFramebuffers = malloc(swapchainImageCount * sizeof(VkFramebuffer));

  for (size_t i = 0; i < swapchainImageCount; i++) {
    VkImageView attachments[] = {swapchainImageViews[i]};

    VkFramebufferCreateInfo framebufferCreateInfo = {0};
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass = renderPass;
    framebufferCreateInfo.attachmentCount = 1;
    framebufferCreateInfo.pAttachments = attachments;
    framebufferCreateInfo.width = swapchainExtent.width;
    framebufferCreateInfo.height = swapchainExtent.height;
    framebufferCreateInfo.layers = 1;

    MW_ASSERT(vkCreateFramebuffer(device, &framebufferCreateInfo, NULL,
                                  &swapchainFramebuffers[i]) == VK_SUCCESS);
  }
}

void createCommandPool() {
  struct QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

  VkCommandPoolCreateInfo commandPoolCreateInfo = {0};
  commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  commandPoolCreateInfo.queueFamilyIndex = indices.graphicsFamily;

  MW_ASSERT(vkCreateCommandPool(device, &commandPoolCreateInfo, NULL,
                                &commandPool) == VK_SUCCESS);
}

void createCommandBuffer() {
  VkCommandBufferAllocateInfo commandBufferAllocInfo = {0};
  commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocInfo.commandBufferCount = 1;
  commandBufferAllocInfo.commandPool = commandPool;
  commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

  MW_ASSERT(vkAllocateCommandBuffers(device, &commandBufferAllocInfo,
                                     &commandBuffer) == VK_SUCCESS);
}

void createSyncObjects() {
  VkSemaphoreCreateInfo semaphoreCreateInfo = {0};
  semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceCreateInfo = {0};
  fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  MW_ASSERT(vkCreateSemaphore(device, &semaphoreCreateInfo, NULL,
                              &imageAvailableSemaphore) == VK_SUCCESS &&
            vkCreateSemaphore(device, &semaphoreCreateInfo, NULL,
                              &renderFinishedSemaphore) == VK_SUCCESS &&
            vkCreateFence(device, &fenceCreateInfo, NULL, &inFlightFence) ==
                VK_SUCCESS);
}

void recordCommandBuffer(VkCommandBuffer commandBuffer,
                         unsigned int imageIndex) {
  VkCommandBufferBeginInfo commandBufferBeginInfo = {0};
  commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  MW_ASSERT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) ==
            VK_SUCCESS);

  VkRenderPassBeginInfo renderPassBeginInfo = {0};
  renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassBeginInfo.renderPass = renderPass;
  renderPassBeginInfo.framebuffer = swapchainFramebuffers[imageIndex];

  VkOffset2D offset = {0, 0};
  renderPassBeginInfo.renderArea.offset = offset;
  renderPassBeginInfo.renderArea.extent = swapchainExtent;

  VkClearValue clearColor = {{{0, 0, 0, 1}}};
  renderPassBeginInfo.clearValueCount = 1;
  renderPassBeginInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphicsPipeline);

  VkViewport viewport = {0};
  viewport.x = 0;
  viewport.y = 0;
  viewport.width = (float)swapchainExtent.width;
  viewport.height = (float)swapchainExtent.height;
  viewport.minDepth = 0;
  viewport.maxDepth = 1;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor = {0};
  scissor.offset = offset;
  scissor.extent = swapchainExtent;
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  vkCmdDraw(commandBuffer, 3, 1, 0, 0);
  vkCmdEndRenderPass(commandBuffer);
  MW_ASSERT(vkEndCommandBuffer(commandBuffer) == VK_SUCCESS);
}

void initVulkan() {
  createInstance();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createSwapchain();
  createImageViews();
  createRenderPass();
  createGraphicsPipeline();
  createFramebuffers();
  createCommandPool();
  createCommandBuffer();
  createSyncObjects();
}

void drawFrame() {
  vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
  vkResetFences(device, 1, &inFlightFence);

  unsigned int imageIndex;
  vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAvailableSemaphore,
                        VK_NULL_HANDLE, &imageIndex);

  vkResetCommandBuffer(commandBuffer, 0);
  recordCommandBuffer(commandBuffer, imageIndex);

  VkSubmitInfo submitInfo = {0};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  MW_ASSERT(vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) ==
            VK_SUCCESS);

  VkPresentInfoKHR presentInfo = {0};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapchains[] = {swapchain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapchains;
  presentInfo.pImageIndices = &imageIndex;

  vkQueuePresentKHR(presentQueue, &presentInfo);
}

void mainLoop() {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    drawFrame();
  }

  vkDeviceWaitIdle(device);
}

void cleanup() {
  vkDestroySemaphore(device, imageAvailableSemaphore, NULL);
  vkDestroySemaphore(device, renderFinishedSemaphore, NULL);
  vkDestroyFence(device, inFlightFence, NULL);

  vkDestroyCommandPool(device, commandPool, NULL);

  for (size_t i = 0; i < swapchainImageCount; i++)
    vkDestroyFramebuffer(device, swapchainFramebuffers[i], NULL);

  vkDestroyPipeline(device, graphicsPipeline, NULL);
  vkDestroyPipelineLayout(device, pipelineLayout, NULL);
  vkDestroyRenderPass(device, renderPass, NULL);

  for (size_t i = 0; i < swapchainImageCount; i++)
    vkDestroyImageView(device, swapchainImageViews[i], NULL);

  vkDestroySwapchainKHR(device, swapchain, NULL);
  vkDestroyDevice(device, NULL);
  vkDestroySurfaceKHR(instance, surface, NULL);
  vkDestroyInstance(instance, NULL);

  glfwDestroyWindow(window);
  glfwTerminate();

  free(swapchainFramebuffers);
  free(swapchainImageViews);
  free(swapchainImages);
}

int main() {
  initWindow();
  initVulkan();

  mainLoop();

  cleanup();
  exit(EXIT_SUCCESS);
}