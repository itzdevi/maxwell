#include "mw_draw_command.h"

#include "core/mw_queue.h"
#include "mw_assert.h"
#include "resources/mw_buffer.h"

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

void mw_renderer_record_command_buffer(
    mw_swapchain swapchain, mw_pipeline pipeline,
    mw_command_buffers commandBuffer, VkFramebuffer* framebuffers,
    mw_buffer buffer, unsigned int imageIndex, unsigned int frameIndex) {
  VkCommandBufferBeginInfo commandBufferBeginInfo = {0};
  commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  MW_ASSERT(vkBeginCommandBuffer(commandBuffer.buffers[frameIndex],
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

  vkCmdBeginRenderPass(commandBuffer.buffers[frameIndex], &renderPassBeginInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer.buffers[frameIndex],
                    VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

  VkViewport viewport = {0};
  viewport.x = 0;
  viewport.y = 0;
  viewport.width = (float)swapchain.extent.width;
  viewport.height = (float)swapchain.extent.height;
  viewport.minDepth = 0;
  viewport.maxDepth = 1;
  vkCmdSetViewport(commandBuffer.buffers[frameIndex], 0, 1, &viewport);

  VkRect2D scissor = {0};
  scissor.offset = offset;
  scissor.extent = swapchain.extent;
  vkCmdSetScissor(commandBuffer.buffers[frameIndex], 0, 1, &scissor);

  VkBuffer buffers[] = {buffer.buffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer.buffers[frameIndex], 0, 1, buffers,
                         offsets);
  vkCmdDraw(commandBuffer.buffers[frameIndex], buffer.size / buffer.stride, 1,
            0, 0);

  vkCmdEndRenderPass(commandBuffer.buffers[frameIndex]);
  MW_ASSERT(vkEndCommandBuffer(commandBuffer.buffers[frameIndex]) ==
            VK_SUCCESS);
}

mw_command_buffers mw_renderer_create_command_buffers(
    mw_device device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
    const unsigned int maxFramesInFlight) {
  mw_command_buffers buffers;
  buffers.buffers = malloc(maxFramesInFlight * sizeof(VkCommandBuffer));
  buffers.pool = mw_create_command_pool(device.device, physicalDevice, surface);

  VkCommandBufferAllocateInfo commandBufferAllocInfo = {0};
  commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocInfo.commandBufferCount = maxFramesInFlight;
  commandBufferAllocInfo.commandPool = buffers.pool;
  commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

  MW_ASSERT(vkAllocateCommandBuffers(device.device, &commandBufferAllocInfo,
                                     buffers.buffers) == VK_SUCCESS);

  return buffers;
}

void mw_renderer_destroy_command_buffers(mw_device device,
                                         mw_command_buffers buffers) {
  vkDestroyCommandPool(device.device, buffers.pool, NULL);

  free(buffers.buffers);
}