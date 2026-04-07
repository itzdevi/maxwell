#include "mw_draw_command.h"

#include <mw_core.h>

#include "mw_renderer.h"
#include "resources/mw_buffer.h"
#include "resources/mw_push_constants.h"

void mw_renderer_record_draw_command_buffer(mw_renderer* renderer,
                                            unsigned int imageIndex) {
  VkCommandBufferBeginInfo commandBufferBeginInfo = {0};
  commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  VkCommandBuffer currentCommandBuffer =
      renderer->draw_command.buffers[renderer->current_frame];

  MW_ASSERT(vkBeginCommandBuffer(currentCommandBuffer,
                                 &commandBufferBeginInfo) == VK_SUCCESS);

  VkClearValue clearValues[2];
  clearValues[0].color = (VkClearColorValue){0.53, 0.81, 0.92, 1.0};
  clearValues[1].depthStencil = (VkClearDepthStencilValue){1.0, 0.0};

  VkRenderPassBeginInfo renderPassBeginInfo = {0};
  renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassBeginInfo.renderPass = renderer->pipeline.render_pass;
  renderPassBeginInfo.framebuffer = renderer->framebuffers[imageIndex];
  renderPassBeginInfo.clearValueCount = 2;
  renderPassBeginInfo.pClearValues = clearValues;

  VkOffset2D offset = {0, 0};
  renderPassBeginInfo.renderArea.offset = offset;
  renderPassBeginInfo.renderArea.extent = renderer->swapchain.extent;

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

  VkBuffer buffers[] = {renderer->vertex_buffer.buffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(currentCommandBuffer, 0, 1, buffers, offsets);
  vkCmdBindIndexBuffer(currentCommandBuffer, renderer->index_buffer.buffer, 0,
                       VK_INDEX_TYPE_UINT32);

  mw_render_pass renderPass;
  while (mw_renderer_pop_render_pass(renderer, &renderPass)) {
    mw_mesh mesh = renderer->meshes[renderPass.mesh_index];

    mw_push_constants_data pushConstants =
        mw_renderer_get_push_constants(renderer, renderPass.transform);
    vkCmdPushConstants(currentCommandBuffer, renderer->pipeline.layout,
                       VK_SHADER_STAGE_VERTEX_BIT, 0,
                       sizeof(mw_push_constants_data), &pushConstants);

    vkCmdDrawIndexed(currentCommandBuffer, mesh.indices_count, 1,
                     mesh.indices_offset, mesh.vertices_offset, 0);
  }

  vkCmdEndRenderPass(currentCommandBuffer);
  MW_ASSERT(vkEndCommandBuffer(currentCommandBuffer) == VK_SUCCESS);
}
