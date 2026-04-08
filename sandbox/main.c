#include <maxwell.h>

int main() {
  mw_engine_info info = {.title = "Sandbox", .size = {800, 600}};
  mw_ctx* engine = mw_engine_create(info);
  mw_engine_run(engine);
  mw_engine_destroy(engine);
}