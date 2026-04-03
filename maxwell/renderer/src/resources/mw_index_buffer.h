#ifndef MW_INDEX_BUFFER_H
#define MW_INDEX_BUFFER_H

#include <mw_gpu.h>

#include "resources/mw_buffer.h"

mw_buffer mw_renderer_create_index_buffer(mw_renderer* renderer,
                                          uint32_t* indices,
                                          uint32_t indicesCount, void* pData);

#endif