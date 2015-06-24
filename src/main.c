#include <pebble.h>
#include "home.h"

void init() {
  show_home();
}

void deinit() {
  hide_home();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}