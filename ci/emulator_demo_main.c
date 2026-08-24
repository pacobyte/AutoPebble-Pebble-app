#include "main_prod.inc"

static void demo_show_primary_quick(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  screen->labelTitle = resetString(screen->labelTitle, "Control Hub");
  screen->labelTop = resetString(screen->labelTop, "Bedroom & Hallway Lights");
  screen->labelMiddle = resetString(screen->labelMiddle, "Open the Garage Door");
  screen->labelBottom = resetString(screen->labelBottom, "Upstairs Climate Control");

  /* Unique actions let CI prove that the production click recognizers still
     distinguish single, long and multi-click events while ActionBarLayer owns
     the native raw-button press animation. */
  screen->actionTop = resetString(screen->actionTop, "CI_SINGLE_UP");
  screen->longActionTop = resetString(screen->longActionTop, "CI_LONG_UP");
  screen->multiActionTop = resetString(screen->multiActionTop, "CI_MULTI_UP");
  screen->actionMiddle = resetString(screen->actionMiddle, "CI_SINGLE_SELECT");
  screen->actionBottom = resetString(screen->actionBottom, "CI_SINGLE_DOWN");

  finishQuickScreen(window);
  APP_LOG(APP_LOG_LEVEL_INFO, "CI_PRIMARY_READY");
}

static void demo_show_secondary_quick(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  screen->labelTop = resetString(screen->labelTop, "Secondary Up");
  screen->labelMiddle = resetString(screen->labelMiddle, "Secondary Select");
  screen->labelBottom = resetString(screen->labelBottom, "Secondary Down");
  screen->actionMiddle = resetString(screen->actionMiddle, "CI_SECOND_SELECT");

  finishQuickScreen(window);
  APP_LOG(APP_LOG_LEVEL_INFO, "CI_SECONDARY_READY");
}

int main(void) {
  Window *keeper = window_create();
  window_stack_push(keeper, false);

  /* The real click handlers send AppMessage actions. Opening AppMessage keeps
     those sends on the production path while the emulator test remains
     independent of Bluetooth/phone state and production init(). */
  app_message_open(256, 256);

  app_timer_register(500, demo_show_primary_quick, NULL);
  app_timer_register(20000, demo_show_secondary_quick, NULL);

  app_event_loop();
  window_destroy(keeper);
  return 0;
}
