#include "main_prod.inc"

static void demo_show_quick_long(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  screen->labelTitle = resetString(screen->labelTitle, "Quick Screen Long Title Test");
  screen->labelTop = resetString(screen->labelTop, "UP: Bedroom & Hallway Lights");
  screen->labelMiddle = resetString(screen->labelMiddle, "SELECT: Open the Garage Door");
  screen->labelBottom = resetString(screen->labelBottom, "DOWN: Upstairs Climate Control");

  finishQuickScreen(window);
}

static void demo_show_quick_no_title(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  /* Explicitly request the legacy-size font to verify caller overrides still win. */
  window->textFont = resetString(window->textFont, FONT_KEY_GOTHIC_18_BOLD);
  screen->labelTop = resetString(screen->labelTop, "Lights");
  screen->labelMiddle = resetString(screen->labelMiddle, "Garage");
  screen->labelBottom = resetString(screen->labelBottom, "Climate");

  finishQuickScreen(window);
}

static void demo_show_list_long(void *context) {
  AutoPebbleWindow *window = initList();
  AutoPebbleList *screen = getCurrentAutoPebbleList();

  screen->headerText = resetString(screen->headerText, "Control Hub Long Header Test");
  screen->labels = setItemInArray(screen->labels, 30, 0, "Living Room Lights and Lamps");
  screen->labels = setItemInArray(screen->labels, 30, 1, "Garage Door and Exterior Lights");
  screen->labels = setItemInArray(screen->labels, 30, 2, "Upstairs Climate Control");
  screen->labels = setItemInArray(screen->labels, 30, 3, "Sleep Mode");
  screen->labels = setItemInArray(screen->labels, 30, 4, "Find Phone");
  screen->labels = setItemInArray(screen->labels, 30, 5, "Extremely Long List Item Name for Ellipsis Testing");
  screen->labels = setItemInArray(screen->labels, 30, 6, "Kitchen");
  screen->labels = setItemInArray(screen->labels, 30, 7, "Front Door");
  screen->labels = setItemInArray(screen->labels, 30, 8, "Back Yard");
  screen->labels = setItemInArray(screen->labels, 30, 9, "All Off");
  screen->size = 10;

  finishList(window);
}

static void demo_show_text_long(void *context) {
  AutoPebbleWindow *window = initTextScreen();
  AutoPebbleTextScreen *screen = (AutoPebbleTextScreen *)getCurrentAutoPebbleItem();

  screen->labelTitle = resetString(screen->labelTitle, "Long Text Screen Title Test");
  screen->labelText = resetString(
      screen->labelText,
      "Pebble Time 2 native 200x228 scrolling test.\n\n"
      "Paragraph one exercises line wrapping across the wider Emery display. "
      "The content should remain readable without clipping at either edge.\n\n"
      "Paragraph two makes the document taller than the viewport so the Down "
      "button must scroll to additional content. This lets CI verify real "
      "ScrollLayer input rather than only static rendering.\n\n"
      "Paragraph three is here to provide a clear visual endpoint after several "
      "scroll operations. The bottom of this paragraph should not be visible "
      "in the initial screenshot.");

  finishTextScreen(window);
}

static void demo_show_list_custom(void *context) {
  AutoPebbleWindow *window = initList();
  AutoPebbleList *screen = getCurrentAutoPebbleList();

  window->titleFont = resetString(window->titleFont, FONT_KEY_GOTHIC_14_BOLD);
  window->textFont = resetString(window->textFont, FONT_KEY_GOTHIC_14);
  screen->defaultCellSize = 25;
  screen->headerText = resetString(screen->headerText, "Explicit Legacy Overrides");
  screen->labels = setItemInArray(screen->labels, 30, 0, "14px font / 25px row");
  screen->labels = setItemInArray(screen->labels, 30, 1, "Caller values should win");
  screen->labels = setItemInArray(screen->labels, 30, 2, "No Emery override here");
  screen->labels = setItemInArray(screen->labels, 30, 3, "Compatibility check");
  screen->labels = setItemInArray(screen->labels, 30, 4, "Final row");
  screen->size = 5;

  finishList(window);
}

int main(void) {
  /* CI-only: keep one inert Window on the stack so Pebble does not return to
     system UI before the first demo timer fires. We still intentionally skip
     production init(), which subscribes to phone/Bluetooth state. */
  Window *keeper = window_create();
  window_stack_push(keeper, false);

  app_timer_register(500, demo_show_quick_long, NULL);
  app_timer_register(25000, demo_show_quick_no_title, NULL);
  app_timer_register(60000, demo_show_list_long, NULL);
  app_timer_register(110000, demo_show_text_long, NULL);
  app_timer_register(160000, demo_show_list_custom, NULL);

  app_event_loop();
  window_destroy(keeper);
  return 0;
}
