#include "mw_command_buffer.h"

#include "mw_assert.h"
#include "mw_render_queue.h"

VkCommandPool mw_create_command_pool(VkDevice device,
                                     VkPhysicalDevice physicalDevice,
                                     VkSurfaceKHR surface) {
  mw_queue_family_indices indices =
      mw_renderer_find_queue_families(physicalDevice, surface);

  VkCommandPoolCreateInfo commandPoolCreateInfo = {0};
  commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  commandPoolCreateInfo.queueFamilyIndex = indices.graphicsFamily;

  VkCommandPool pool;
  MW_ASSERT(vkCreateCommandPool(device, &commandPoolCreateInfo, NULL, &pool) ==
            VK_SUCCESS);
  return pool;
}

void mw_renderer_record_command_buffer(mw_swapchain swapchain,
                                       mw_pipeline pipeline,
                                       mw_command_buffer commandBuffer,
                                       VkFramebuffer* framebuffers,
                                       unsigned int imageIndex) {
  VkCommandBufferBeginInfo commandBufferBeginInfo = {0};
  commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  MW_ASSERT(vkBeginCommandBuffer(commandBuffer.buffer,
                                 &commandBufferBeginInfo) == VK_SUCCESS);

  VkRenderPassBeginInfo renderPassBeginInfo = {0};
  renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassBeginInfo.renderPass = pipeline.renderPass;
  renderPassBeginInfo.framebuffer = framebuffers[imageIndex];

  VkOffset2D offset = {0, 0};
  renderPassBeginInfo.renderArea.offset = offset;
  renderPassBeginInfo.renderArea.extent = swapchain.extent;

  VkClearValue clearColor = {{{0, 0, 0, 1}}};
  renderPassBeginInfo.clearValueCount = 1;
  renderPassBeginInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandBuffer.buffer, &renderPassBeginInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer.buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipeline.pipeline);

  VkViewport viewport = {0};
  viewport.x = 0;
  viewport.y = 0;
  viewport.width = (float)swapchain.extent.width;
  viewport.height = (float)swapchain.extent.height;
  viewport.minDepth = 0;
  viewport.maxDepth = 1;
  vkCmdSetViewport(commandBuffer.buffer, 0, 1, &viewport);

  VkRect2D scissor = {0};
  scissor.offset = offset;
  scissor.extent = swapchain.extent;
  vkCmdSetScissor(commandBuffer.buffer, 0, 1, &scissor);

  vkCmdDraw(commandBuffer.buffer, 3, 1, 0, 0);
  vkCmdEndRenderPass(commandBuffer.buffer);
  MW_ASSERT(vkEndCommandBuffer(commandBuffer.buffer) == VK_SUCCESS);
}

mw_command_buffer mw_renderer_create_command_buffer(
    mw_device device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
  mw_command_buffer buffer;
  buffer.pool = mw_create_command_pool(device.device, physicalDevice, surface);

  VkCommandBufferAllocateInfo commandBufferAllocInfo = {0};
  commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocInfo.commandBufferCount = 1;
  commandBufferAllocInfo.commandPool = buffer.pool;
  commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

  MW_ASSERT(vkAllocateCommandBuffers(device.device, &commandBufferAllocInfo,
                                     &buffer.buffer) == VK_SUCCESS);

  return buffer;
}

void mw_renderer_destroy_command_buffer(mw_device device,
                                        mw_command_buffer buffer) {
  vkDestroyCommandPool(device.device, buffer.pool, NULL);
}