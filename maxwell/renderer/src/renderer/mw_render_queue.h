#ifndef MW_RENDER_PAGE_H
#define MW_RENDER_PAGE_H

#include <cglm/cglm.h>
#include <stdint.h>

#include "mw_renderer.h"

typedef struct mw_render_page {
  mw_draw_item* passes;
  uint32_t length;
} mw_render_page;

void mw_renderer_create_render_page(mw_renderer* renderer);
void mw_renderer_destroy_render_pages(mw_renderer* renderer);
void mw_renderer_clean_render_queue(mw_renderer* renderer);
void mw_renderer_push_draw_item(mw_renderer* renderer, mw_draw_item renderPass);
int mw_renderer_pop_draw_item(mw_renderer* renderer, mw_draw_item* renderPass);

#endif