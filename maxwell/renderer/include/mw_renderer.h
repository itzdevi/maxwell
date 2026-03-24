#ifndef MW_RENDERER_H
#define MW_RENDERER_H

#include "mw_application.h"
#include "mw_renderer_vertex.h"

void mw_renderer_initialize(mw_app_info info, mw_vertex* vertices,
                            uint32_t verticesSize);
void mw_renderer_cleanup();
void mw_renderer_draw_frame();
void mw_renderer_wait_for_idle();
void mw_renderer_update_framebuffer_resized();

#endif