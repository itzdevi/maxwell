#include "mw_render_queue.h"

#include <stdlib.h>
#include <string.h>

#include "core/mw_renderer_def.h"

const unsigned int RENDER_QUEUE_PAGE_SIZE = 1024;

void mw_renderer_create_render_page(mw_renderer* renderer) {
  renderer->render_pages =
      realloc(renderer->render_pages,
              (renderer->render_page_count + 1) * sizeof(mw_render_page));
  renderer->render_pages[renderer->render_page_count].passes =
      malloc(sizeof(mw_draw_item) * RENDER_QUEUE_PAGE_SIZE);
  renderer->render_pages[renderer->render_page_count].length = 0;
  renderer->render_page_count++;
}

void mw_renderer_destroy_render_pages(mw_renderer* renderer) {
  for (size_t i = 0; i < renderer->render_page_count; i++) {
    free(renderer->render_pages[i].passes);
  }
  free(renderer->render_pages);
}

void mw_renderer_clean_render_queue(mw_renderer* renderer) {
  for (size_t i = 0; i < renderer->render_page_count; i++) {
    renderer->render_pages[i].length = 0;
  }
  renderer->current_push_page = 0;
  renderer->current_pop_page = 0;
  renderer->current_page_offset = 0;
}

void mw_renderer_push_draw_item(mw_renderer* renderer,
                                mw_draw_item renderPass) {
  if (!renderer->render_page_count) mw_renderer_create_render_page(renderer);

  if (renderer->render_pages[renderer->current_push_page].length ==
      RENDER_QUEUE_PAGE_SIZE) {
    renderer->current_push_page++;
  }
  if (renderer->render_page_count == renderer->current_push_page)
    mw_renderer_create_render_page(renderer);

  mw_render_page* currentPage =
      &renderer->render_pages[renderer->current_push_page];
  currentPage->passes[currentPage->length++] = renderPass;
}

int mw_renderer_pop_draw_item(mw_renderer* renderer, mw_draw_item* renderPass) {
  if (!renderer->render_page_count) return 0;

  mw_render_page* currentPage =
      &renderer->render_pages[renderer->current_pop_page];

  while (!currentPage->length) {
    renderer->current_pop_page++;
    renderer->current_page_offset = 0;
    currentPage = &renderer->render_pages[renderer->current_pop_page];

    if (renderer->current_pop_page == renderer->render_page_count) return 0;
  }

  *renderPass = currentPage->passes[renderer->current_page_offset++];
  currentPage->length--;

  return 1;
}