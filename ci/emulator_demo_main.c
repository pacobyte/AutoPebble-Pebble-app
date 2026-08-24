#include "main_prod.inc"

static AutoPebbleWindow *primaryWindow = NULL;

static void demo_show_primary_direct(void *context) {
  primaryWindow = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  screen->labelTitle = resetString(screen->labelTitle, "Control Hub");
  screen->labelTop = resetString(screen->labelTop, "Bedroom & Hallway Lights");
  screen->labelMiddle = resetString(screen->labelMiddle, "Open the Garage Door");
  screen->labelBottom = resetString(screen->labelBottom, "Upstairs Climate Control");

  /* Before finishQuickScreen(), initQuickScreen() is still using AutoPebble's
     inherited direct Window click provider. This gives CI an A/B baseline for
     multi-click recognition before ActionBarLayer owns the PT2 click provider. */
  screen->actionTop = resetString(screen->actionTop, "CI_DIRECT_SINGLE_UP");
  screen->longActionTop = resetString(screen->longActionTop, "CI_DIRECT_LONG_UP");
  screen->multiActionTop = resetString(screen->multiActionTop, "CI_DIRECT_MULTI_UP");

  APP_LOG(APP_LOG_LEVEL_INFO, "CI_DIRECT_READY");
}

static void demo_enable_primary_actionbar(void *context) {
  if (!primaryWindow) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "CI_ACTIONBAR_NO_PRIMARY");
    return;
  }

  AutoPebbleQuickScreen *screen = getAutoPebbleQuickScreen(primaryWindow);
  screen->actionTop = resetString(screen->actionTop, "CI_SINGLE_UP");
  screen->longActionTop = resetString(screen->longActionTop, "CI_LONG_UP");
  screen->multiActionTop = resetString(screen->multiActionTop, "CI_MULTI_UP");
  screen->actionMiddle = resetString(screen->actionMiddle, "CI_SINGLE_SELECT");
  screen->actionBottom = resetString(screen->actionBottom, "CI_SINGLE_DOWN");

  /* This is the real PT2 production renderer and native ActionBar path. */
  finishQuickScreen(primaryWindow);
  APP_LOG(APP_LOG_LEVEL_INFO, "CI_ACTIONBAR_READY");
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

  app_timer_register(500, demo_show_primary_direct, NULL);
  app_timer_register(9000, demo_enable_primary_actionbar, NULL);
  app_timer_register(22000, demo_show_secondary_quick, NULL);

  app_event_loop();
  window_destroy(keeper);
  return 0;
}
