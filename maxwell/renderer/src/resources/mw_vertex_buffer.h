#ifndef MW_VERTEX_BUFFER_H
#define MW_VERTEX_BUFFER_H

#include <mw_gpu.h>
#include <vulkan/vulkan.h>

#include "resources/mw_buffer.h"

mw_buffer mw_renderer_create_vertex_buffer(mw_renderer* renderer,
                                           mw_vertex* vertices,
                                           uint32_t verticesCount, void* pData);

#endif