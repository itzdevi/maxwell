#ifndef MW_VERTEX_BUFFER_H
#define MW_VERTEX_BUFFER_H

#include <vulkan/vulkan.h>

#include "commands/mw_command.h"
#include "core/mw_device.h"
#include "mw_buffer.h"

mw_buffer mw_renderer_create_vertex_buffer(
    mw_device device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
    mw_vertex* vertices, uint32_t verticesCount, mw_command transferCommand,
    void* pData);

#endif