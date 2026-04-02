#include "mw_render_queue.h"

#include <stdlib.h>
#include <string.h>

#include "core/mw_renderer_def.h"

const unsigned int RENDER_QUEUE_PAGE_SIZE = 1024;

void mw_renderer_create_render_page(mw_renderer* renderer) {
  renderer->renderPages =
      realloc(renderer->renderPages,
              (renderer->renderPagesCount + 1) * sizeof(mw_render_page));
  renderer->renderPages[renderer->renderPagesCount].passes =
      malloc(sizeof(mw_render_pass) * RENDER_QUEUE_PAGE_SIZE);
  renderer->renderPages[renderer->renderPagesCount].length = 0;
  renderer->renderPagesCount++;
}

void mw_renderer_destroy_render_pages(mw_renderer* renderer) {
  for (size_t i = 0; i < renderer->renderPagesCount; i++) {
    free(renderer->renderPages[i].passes);
  }
  free(renderer->renderPages);
}

void mw_renderer_clean_render_queue(mw_renderer* renderer) {
  for (size_t i = 0; i < renderer->renderPagesCount; i++) {
    renderer->renderPages[i].length = 0;
  }
  renderer->currentPushPage = 0;
  renderer->currentPopPage = 0;
  renderer->currentPageOffset = 0;
}

void mw_renderer_push_render_pass(mw_renderer* renderer,
                                  mw_render_pass renderPass) {
  if (!renderer->renderPagesCount) mw_renderer_create_render_page(renderer);

  if (renderer->renderPages[renderer->currentPushPage].length ==
      RENDER_QUEUE_PAGE_SIZE) {
    renderer->currentPushPage++;
  }
  if (renderer->renderPagesCount == renderer->currentPushPage)
    mw_renderer_create_render_page(renderer);

  mw_render_page* currentPage =
      &renderer->renderPages[renderer->currentPushPage];
  currentPage->passes[currentPage->length++] = renderPass;
}

int mw_renderer_pop_render_pass(mw_renderer* renderer,
                                mw_render_pass* renderPass) {
  if (!renderer->renderPagesCount) return 0;

  mw_render_page* currentPage =
      &renderer->renderPages[renderer->currentPopPage];

  while (!currentPage->length) {
    renderer->currentPopPage++;
    renderer->currentPageOffset = 0;
    currentPage = &renderer->renderPages[renderer->currentPopPage];

    if (renderer->currentPopPage == renderer->renderPagesCount) return 0;
  }

  *renderPass = currentPage->passes[renderer->currentPageOffset++];
  currentPage->length--;

  return 1;
}