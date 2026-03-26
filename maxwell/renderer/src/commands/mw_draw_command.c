#include "mw_draw_command.h"

#include "core/mw_queue.h"
#include "mw_assert.h"
#include "resources/mw_buffer.h"

void mw_renderer_record_draw_command_buffer(
    mw_swapchain swapchain, mw_pipeline pipeline, mw_command commandBuffer,
    VkFramebuffer* framebuffers, mw_buffer vertexBuffer, mw_buffer indexBuffer,
    unsigned int imageIndex, unsigned int frameIndex) {
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

  VkBuffer buffers[] = {vertexBuffer.buffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer.buffers[frameIndex], 0, 1, buffers,
                         offsets);
  vkCmdBindIndexBuffer(commandBuffer.buffers[frameIndex], indexBuffer.buffer, 0,
                       VK_INDEX_TYPE_UINT16);
  vkCmdDrawIndexed(commandBuffer.buffers[frameIndex],
                   indexBuffer.size / indexBuffer.stride, 1, 0, 0, 0);

  vkCmdEndRenderPass(commandBuffer.buffers[frameIndex]);
  MW_ASSERT(vkEndCommandBuffer(commandBuffer.buffers[frameIndex]) ==
            VK_SUCCESS);
}
