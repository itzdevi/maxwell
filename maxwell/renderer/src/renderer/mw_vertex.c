#include <vulkan/vulkan.h>

#include "mw_renderer_vertex.h"

VkVertexInputBindingDescription mw_renderer_get_vertex_binding_description() {
  VkVertexInputBindingDescription bindingDescription = {0};
  bindingDescription.binding = 0;
  bindingDescription.stride = sizeof(mw_vertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  return bindingDescription;
}

void mw_renderer_get_vertex_attribute_description(
    VkVertexInputAttributeDescription* attributeDescriptions) {
  uint32_t location = 0;

  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = location++;
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(mw_vertex, position);

  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = location++;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(mw_vertex, normal);

  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = location++;
  attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(mw_vertex, uv);
}