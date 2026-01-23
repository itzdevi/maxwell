#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "assert.h"

VkRenderPass renderPass;
VkFramebuffer* swapchainFramebuffers;
VkCommandPool commandPool;
VkCommandBuffer commandBuffer;
VkSemaphore imageAvailableSemaphore;
VkSemaphore renderFinishedSemaphore;
VkFence inFlightFence;

unsigned int swapchainImageCount;

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