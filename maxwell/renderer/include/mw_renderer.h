#ifndef MW_RENDERER_H
#define MW_RENDERER_H

#include "mw_application.h"

void mw_renderer_initialize(mw_app_info info);
void mw_renderer_cleanup();
void mw_renderer_draw_frame();
void mw_renderer_wait_for_idle();
void mw_renderer_update_framebuffer_resized();

#endif