#include "mw_vertex_buffer.h"

#include "mw_assert.h"

VkBuffer mw_renderer_create_vertex_buffer(mw_device device,
                                          uint32_t verticesSize) {
  VkBufferCreateInfo bufferInfo = {0};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = verticesSize * sizeof(mw_vertex);
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VkBuffer vertexBuffer;
  MW_ASSERT(vkCreateBuffer(device.device, &bufferInfo, NULL, &vertexBuffer) ==
            VK_SUCCESS);

  return vertexBuffer;
}

void mw_renderer_destroy_vertex_buffer(mw_device device,
                                       VkBuffer vertexBuffer) {
  vkDestroyBuffer(device.device, vertexBuffer, NULL);
}