#include "api/mw_input.h"

#include <glfw/glfw3.h>

#include "core/mw_engine_def.h"

int mw_key_pressed(mw_iter* it, int key) {
  return glfwGetKey(mw_engine_get_ctx(it)->window, key);
}

void mw_mouse_position(mw_iter* it, vec2 dest) {
  mw_ctx* ctx = mw_engine_get_ctx(it);

  glm_vec2_copy((vec2){ctx->cursor_x, ctx->cursor_y}, dest);
}

void mw_mouse_delta(mw_iter* it, vec2 dest) {
  mw_ctx* ctx = mw_engine_get_ctx(it);

  if (ctx->last_cursor_x == -1) {
    glm_vec2_zero(dest);
    return;
  }

  glm_vec2_copy((vec2){ctx->cursor_x - ctx->last_cursor_x,
                       ctx->cursor_y - ctx->last_cursor_y},
                dest);
}

void mw_cursor_mode_it(mw_iter* it, int mode) {
  mw_cursor_mode(mw_engine_get_ctx(it), mode);
}

void mw_cursor_mode(mw_ctx* ctx, int mode) {
  glfwSetInputMode(ctx->window, GLFW_CURSOR, mode);
}