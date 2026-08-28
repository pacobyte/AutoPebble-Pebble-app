#include "main_prod.inc"

static void demo_show_primary_quick(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  screen->labelTitle = resetString(screen->labelTitle, "Control Hub");
  screen->labelTop = resetString(screen->labelTop, "Bedroom & Hallway Lights");
  screen->labelMiddle = resetString(screen->labelMiddle, "Open the Garage Door");
  screen->labelBottom = resetString(screen->labelBottom, "Upstairs Climate Control");

  screen->actionTop = resetString(screen->actionTop, "CI_SINGLE_UP");
  screen->longActionTop = resetString(screen->longActionTop, "CI_LONG_UP");
  screen->multiActionTop = resetString(screen->multiActionTop, "CI_MULTI_UP");
  screen->actionMiddle = resetString(screen->actionMiddle, "CI_SINGLE_SELECT");
  screen->actionBottom = resetString(screen->actionBottom, "CI_SINGLE_DOWN");

  /* Exercise the real production renderer on the active emulator platform. */
  finishQuickScreen(window);
  if (screen->actionBar) {
    APP_LOG(APP_LOG_LEVEL_INFO, "CI_NATIVE_ACTIONBAR_ACTIVE");
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "CI_NATIVE_ACTIONBAR_INACTIVE");
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "CI_PRIMARY_READY");
}

static void demo_show_secondary_quick(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  /* Intentionally omit the title so this also covers the no-title layout. */
  screen->labelTop = resetString(screen->labelTop, "Secondary Up");
  screen->labelMiddle = resetString(screen->labelMiddle, "Secondary Select");
  screen->labelBottom = resetString(screen->labelBottom, "Secondary Down");
  screen->actionMiddle = resetString(screen->actionMiddle, "CI_SECOND_SELECT");

  finishQuickScreen(window);
  APP_LOG(APP_LOG_LEVEL_INFO, "CI_SECONDARY_READY");
}


static void demo_show_media_volume_quick(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  /* Reproduce the first physical-PT2 clipping report closely enough to catch
     baseline/descender regressions in the emulator artifact set. */
  screen->labelTitle = resetString(screen->labelTitle, "Media Volume");
  screen->labelTop = resetString(screen->labelTop, "+5%");
  screen->labelMiddle = resetString(screen->labelMiddle, "Nothing Playing · --%");
  screen->labelBottom = resetString(screen->labelBottom, "-5%");

  finishQuickScreen(window);
  APP_LOG(APP_LOG_LEVEL_INFO, "CI_MEDIA_VOLUME_READY");
}


static void demo_show_text_screen(void *context) {
  AutoPebbleWindow *window = initTextScreen();
  AutoPebbleTextScreen *screen = (AutoPebbleTextScreen *)getAutoPebbleItem(window);

  screen->labelTitle = resetString(screen->labelTitle, "Physical PT2 Text");
  screen->labelText = resetString(screen->labelText,
      "Descender check: Playing, paging, volume, glyphs. "
      "This body is intentionally long enough to exercise the native PT2 "
      "paging-aware Text Screen layout without changing the production path.");

  finishTextScreen(window);
  APP_LOG(APP_LOG_LEVEL_INFO, "CI_TEXT_SCREEN_READY");
}

static void demo_show_list_screen(void *context) {
  AutoPebbleWindow *window = initList();
  AutoPebbleList *screen = (AutoPebbleList *)getAutoPebbleItem(window);

  screen->headerText = resetString(screen->headerText, "Control Hub");
  screen->labels = setItemInArray(screen->labels, 30, 0, "Sleep");
  screen->labels = setItemInArray(screen->labels, 30, 1, "Phones");
  screen->labels = setItemInArray(screen->labels, 30, 2, "Audio & Media");
  screen->labels = setItemInArray(screen->labels, 30, 3, "A deliberately long list label");
  screen->size = 4;

  finishList(window);
  APP_LOG(APP_LOG_LEVEL_INFO, "CI_LIST_SCREEN_READY");
}

int main(void) {
  Window *keeper = window_create();
  window_stack_push(keeper, false);

  /* The real click handlers send AppMessage actions. Opening AppMessage keeps
     those sends on the production path while the emulator test remains
     independent of Bluetooth/phone state and production init(). */
  app_message_open(256, 256);

  /* pebble install launches the app before CI attaches `pebble logs`. Keep the
     first screen comfortably after logger attachment, then leave a generous
     interaction window before stacking the secondary screen. */
  app_timer_register(7000, demo_show_primary_quick, NULL);
  app_timer_register(22000, demo_show_secondary_quick, NULL);
  app_timer_register(32000, demo_show_media_volume_quick, NULL);
  app_timer_register(42000, demo_show_text_screen, NULL);
  app_timer_register(52000, demo_show_list_screen, NULL);

  app_event_loop();
  window_destroy(keeper);
  return 0;
}
