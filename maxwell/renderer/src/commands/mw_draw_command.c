#include "mw_draw_command.h"

#include <mw_core.h>

#include "mw_renderer.h"
#include "resources/mw_buffer.h"

void mw_renderer_record_draw_command_buffer(mw_renderer* renderer,
                                            mw_buffer vertexBuffer,
                                            mw_buffer indexBuffer,
                                            unsigned int imageIndex) {
  VkCommandBufferBeginInfo commandBufferBeginInfo = {0};
  commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  VkCommandBuffer currentCommandBuffer =
      renderer->drawCommand.buffers[renderer->currentFrame];

  MW_ASSERT(vkBeginCommandBuffer(currentCommandBuffer,
                                 &commandBufferBeginInfo) == VK_SUCCESS);

  VkRenderPassBeginInfo renderPassBeginInfo = {0};
  renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassBeginInfo.renderPass = renderer->pipeline.renderPass;
  renderPassBeginInfo.framebuffer = renderer->framebuffers[imageIndex];

  VkOffset2D offset = {0, 0};
  renderPassBeginInfo.renderArea.offset = offset;
  renderPassBeginInfo.renderArea.extent = renderer->swapchain.extent;

  VkClearValue clearColor = {{{0, 0, 0, 1}}};
  renderPassBeginInfo.clearValueCount = 1;
  renderPassBeginInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(currentCommandBuffer, &renderPassBeginInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(currentCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    renderer->pipeline.pipeline);

  VkViewport viewport = {0};
  viewport.x = 0;
  viewport.y = 0;
  viewport.width = (float)renderer->swapchain.extent.width;
  viewport.height = (float)renderer->swapchain.extent.height;
  viewport.minDepth = 0;
  viewport.maxDepth = 1;
  vkCmdSetViewport(currentCommandBuffer, 0, 1, &viewport);

  VkRect2D scissor = {0};
  scissor.offset = offset;
  scissor.extent = renderer->swapchain.extent;
  vkCmdSetScissor(currentCommandBuffer, 0, 1, &scissor);

  VkBuffer buffers[] = {vertexBuffer.buffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(currentCommandBuffer, 0, 1, buffers, offsets);
  vkCmdBindIndexBuffer(currentCommandBuffer, indexBuffer.buffer, 0,
                       VK_INDEX_TYPE_UINT16);
  vkCmdDrawIndexed(currentCommandBuffer, indexBuffer.size / indexBuffer.stride,
                   1, 0, 0, 0);

  vkCmdEndRenderPass(currentCommandBuffer);
  MW_ASSERT(vkEndCommandBuffer(currentCommandBuffer) == VK_SUCCESS);
}
