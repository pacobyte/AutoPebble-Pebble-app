#include "main_prod.inc"

static void demo_show_quick(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  screen->labelTitle = resetString(screen->labelTitle, "Quick Screen");
  screen->labelTop = resetString(screen->labelTop, "UP: Lights");
  screen->labelMiddle = resetString(screen->labelMiddle, "SELECT: Garage");
  screen->labelBottom = resetString(screen->labelBottom, "DOWN: Climate");

  finishQuickScreen(window);
}

static void demo_show_list(void *context) {
  AutoPebbleWindow *window = initList();
  AutoPebbleList *screen = getCurrentAutoPebbleList();

  screen->headerText = resetString(screen->headerText, "Control Hub");
  screen->labels = setItemInArray(screen->labels, 30, 0, "Living Room");
  screen->labels = setItemInArray(screen->labels, 30, 1, "Garage Door");
  screen->labels = setItemInArray(screen->labels, 30, 2, "Climate");
  screen->labels = setItemInArray(screen->labels, 30, 3, "Sleep Mode");
  screen->labels = setItemInArray(screen->labels, 30, 4, "Find Phone");
  screen->labels = setItemInArray(screen->labels, 30, 5, "Longer Item Name Test");
  screen->size = 6;

  finishList(window);
}

static void demo_show_text(void *context) {
  AutoPebbleWindow *window = initTextScreen();
  AutoPebbleTextScreen *screen = (AutoPebbleTextScreen *)getCurrentAutoPebbleItem();

  screen->labelTitle = resetString(screen->labelTitle, "Text Screen");
  screen->labelText = resetString(
      screen->labelText,
      "Pebble Time 2 native 200x228 layout test.\n\n"
      "This sample is intentionally long enough to exercise wrapping, "
      "scrolling, title sizing, and the wider Emery display.");

  finishTextScreen(window);
}

int main(void) {
  init();

  app_timer_register(500, demo_show_quick, NULL);
  app_timer_register(4500, demo_show_list, NULL);
  app_timer_register(8500, demo_show_text, NULL);

  app_event_loop();
  deinit();
  return 0;
}
