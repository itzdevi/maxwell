#ifndef MW_INDEX_BUFFER_H
#define MW_INDEX_BUFFER_H

#include "mw_buffer.h"

mw_buffer mw_renderer_create_index_buffer(
    mw_device device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
    uint16_t* indices, uint32_t indicesCount, mw_command transferCommand,
    void* pData);

#endif