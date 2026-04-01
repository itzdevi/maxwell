#ifndef MW_VERTEX_H
#define MW_VERTEX_H

#include <cglm/cglm.h>
#include <vulkan/vulkan.h>

typedef struct mw_vertex {
  vec3 position;
  vec3 normal;
  vec2 uv;
} mw_vertex;

VkVertexInputBindingDescription mw_renderer_get_vertex_binding_description();
void mw_renderer_get_vertex_attribute_description(
    VkVertexInputAttributeDescription* attributeDescriptions);

#endif