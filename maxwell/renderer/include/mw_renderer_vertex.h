#ifndef MW_VERTEX_H
#define MW_VERTEX_H

#include <vulkan/vulkan.h>

#include "mw_math.h"

typedef struct mw_vertex {
  mw_vec2 position;
  mw_vec3 color;
} mw_vertex;

VkVertexInputBindingDescription mw_renderer_get_vertex_binding_description();
void mw_renderer_get_vertex_attribute_description(
    VkVertexInputAttributeDescription* attributeDescriptions);

#endif