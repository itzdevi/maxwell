#include <maxwell.h>

void onLoad() { printf("hi\n"); }

int main() {
  mw_engine_info info = {"Sandbox"};
  mw_ctx* engine = mw_engine_init(info);
}