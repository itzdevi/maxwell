#ifndef MW_VERTEX_BUFFER_H
#define MW_VERTEX_BUFFER_H

#include <vulkan/vulkan.h>

#include "mw_device.h"
#include "mw_renderer_vertex.h"

VkBuffer mw_renderer_create_vertex_buffer(mw_device device,
                                          uint32_t verticesSize);
void mw_renderer_destroy_vertex_buffer(mw_device device, VkBuffer vertexBuffer);

#endif