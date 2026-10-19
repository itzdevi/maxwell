#include "mw_draw_command.h"

#include <cglm/io.h>
#include <mw_core.h>

#include "mw_renderer.h"
#include "resources/mw_buffer.h"
#include "resources/mw_push_constants.h"

static void transition_image_layout(
    VkCommandBuffer cmd, VkImage image, VkImageLayout oldLayout,
    VkImageLayout newLayout, VkAccessFlags srcAccess, VkAccessFlags dstAccess,
    VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage,
    VkImageAspectFlags aspectMask) {
  VkImageMemoryBarrier barrier = {0};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = aspectMask;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;
  barrier.srcAccessMask = srcAccess;
  barrier.dstAccessMask = dstAccess;
  vkCmdPipelineBarrier(cmd, srcStage, dstStage, 0, 0, NULL, 0, NULL, 1,
                       &barrier);
}

void mw_renderer_record_draw_command_buffer(mw_renderer* renderer,
                                            unsigned int imageIndex) {
  VkCommandBufferBeginInfo commandBufferBeginInfo = {0};
  commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  VkCommandBuffer currentCommandBuffer =
      renderer->draw_command.buffers[renderer->current_frame];

  MW_ASSERT(vkBeginCommandBuffer(currentCommandBuffer,
                                 &commandBufferBeginInfo) == VK_SUCCESS);

  VkImage swapchainImage = renderer->swapchain.images[imageIndex];

  transition_image_layout(
      currentCommandBuffer, swapchainImage, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 0,
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_IMAGE_ASPECT_COLOR_BIT);

  transition_image_layout(
      currentCommandBuffer, renderer->swapchain.depth_image,
      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL, 0,
      VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);

  VkClearValue clearValues[2];
  clearValues[0].color = (VkClearColorValue){0, 0, 0, 1.0};
  clearValues[1].depthStencil = (VkClearDepthStencilValue){1.0, 0.0};

  VkRenderingAttachmentInfo colorAttachment = {0};
  colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
  colorAttachment.imageView = renderer->swapchain.image_views[imageIndex];
  colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.clearValue = clearValues[0];

  VkRenderingAttachmentInfo depthAttachment = {0};
  depthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
  depthAttachment.imageView = renderer->swapchain.depth_image_view;
  depthAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  depthAttachment.clearValue = clearValues[1];

  VkRenderingInfo renderingInfo = {0};
  renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
  renderingInfo.renderArea = (VkRect2D){.extent = renderer->swapchain.extent};
  renderingInfo.layerCount = 1;
  renderingInfo.colorAttachmentCount = 1;
  renderingInfo.pColorAttachments = &colorAttachment;
  renderingInfo.pDepthAttachment = &depthAttachment;

  vkCmdBeginRendering(currentCommandBuffer, &renderingInfo);
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
  scissor.offset = (VkOffset2D){0, 0};
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

    mw_push_constants_data pushConstants = mw_renderer_get_push_constants(
        renderer, renderPass.transform, renderPass.view, renderPass.proj,
        renderPass.viewPos);

    vkCmdPushConstants(currentCommandBuffer, renderer->pipeline.layout,
                       VK_SHADER_STAGE_VERTEX_BIT, 0,
                       sizeof(mw_push_constants_data), &pushConstants);

    vkCmdDrawIndexed(currentCommandBuffer, mesh.indices_count, 1,
                     mesh.indices_offset, mesh.vertices_offset, 0);
  }

  vkCmdEndRendering(currentCommandBuffer);

  transition_image_layout(
      currentCommandBuffer, swapchainImage,
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, 0,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_IMAGE_ASPECT_COLOR_BIT);

  MW_ASSERT(vkEndCommandBuffer(currentCommandBuffer) == VK_SUCCESS);
}
