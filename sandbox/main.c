#include <maxwell.h>

int main() {
  mw_app_info info = {.title = "Sandbox", .size = {800, 600}};
  mw_app_start(info);
}