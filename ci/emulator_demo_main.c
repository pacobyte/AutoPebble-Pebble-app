#include "main_prod.inc"

static void demo_show_quick_with_title(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  screen->labelTitle = resetString(screen->labelTitle, "Control Hub");
  screen->labelTop = resetString(screen->labelTop, "Bedroom & Hallway Lights");
  screen->labelMiddle = resetString(screen->labelMiddle, "Open the Garage Door");
  screen->labelBottom = resetString(screen->labelBottom, "Upstairs Climate Control");

  finishQuickScreen(window);
}

static void demo_show_quick_without_title(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  screen->labelTop = resetString(screen->labelTop, "Bedroom & Hallway Lights");
  screen->labelMiddle = resetString(screen->labelMiddle, "Open the Garage Door");
  screen->labelBottom = resetString(screen->labelBottom, "Upstairs Climate Control");

  finishQuickScreen(window);
}

int main(void) {
  Window *keeper = window_create();
  window_stack_push(keeper, false);

  app_timer_register(500, demo_show_quick_with_title, NULL);
  app_timer_register(30000, demo_show_quick_without_title, NULL);

  app_event_loop();
  window_destroy(keeper);
  return 0;
}
