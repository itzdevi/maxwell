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
  renderer->currentPage = 0;
  renderer->currentPageOffset = 0;
}

void mw_renderer_push_render_pass(mw_renderer* renderer,
                                  mw_render_pass renderPass) {
  if (renderer->renderPages[renderer->currentPage].length ==
      RENDER_QUEUE_PAGE_SIZE) {
    renderer->currentPage++;
  }
  if (renderer->renderPagesCount == renderer->currentPage)
    mw_renderer_create_render_page(renderer);

  mw_render_page* currentPage = &renderer->renderPages[renderer->currentPage];
  currentPage->passes[currentPage->length++] = renderPass;
}

int mw_renderer_pop_render_pass(mw_renderer* renderer,
                                mw_render_pass* renderPass) {
  uint32_t currentPage = 0;
  while (!renderer->renderPages[currentPage].length) {
    currentPage++;
    renderer->currentPageOffset = 0;
  }

  if (currentPage == renderer->renderPagesCount) return 0;

  *renderPass =
      renderer->renderPages[currentPage].passes[renderer->currentPageOffset++];
  renderer->renderPages[currentPage].length--;

  return 1;
}