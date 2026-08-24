#include "main_prod.inc"

static ActionBarLayer *prototypeActionBar = NULL;
static GBitmap *prototypeUp = NULL;
static GBitmap *prototypeSelect = NULL;
static GBitmap *prototypeDown = NULL;

static void apply_actionbar_prototype(AutoPebbleWindow *window, AutoPebbleQuickScreen *screen) {
  Layer *root = window_get_root_layer(window->window);
  GRect bounds = layer_get_bounds(root);
  int textWidth = bounds.size.w - ACTION_BAR_WIDTH;

  prototypeActionBar = action_bar_layer_create();
  prototypeUp = gbitmap_create_with_resource(RESOURCE_ID_ACTIONBAR_UP);
  prototypeSelect = gbitmap_create_with_resource(RESOURCE_ID_ACTIONBAR_SELECT);
  prototypeDown = gbitmap_create_with_resource(RESOURCE_ID_ACTIONBAR_DOWN);

  action_bar_layer_set_icon(prototypeActionBar, BUTTON_ID_UP, prototypeUp);
  action_bar_layer_set_icon(prototypeActionBar, BUTTON_ID_SELECT, prototypeSelect);
  action_bar_layer_set_icon(prototypeActionBar, BUTTON_ID_DOWN, prototypeDown);
  action_bar_layer_add_to_window(prototypeActionBar, window->window);

  TextLayer *layers[4] = {
    screen->textLayerTitle,
    screen->textLayerTop,
    screen->textLayerMiddle,
    screen->textLayerBottom
  };

  for (int i = 0; i < 4; i++) {
    if (!layers[i]) {
      continue;
    }
    Layer *layer = text_layer_get_layer(layers[i]);
    GRect frame = layer_get_frame(layer);
    frame.size.w = textWidth;
    layer_set_frame(layer, frame);
  }

  layer_mark_dirty(root);
}

static void demo_show_quick_long(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  screen->labelTitle = resetString(screen->labelTitle, "ActionBar Prototype");
  screen->labelTop = resetString(screen->labelTop, "Bedroom & Hallway Lights");
  screen->labelMiddle = resetString(screen->labelMiddle, "Open the Garage Door");
  screen->labelBottom = resetString(screen->labelBottom, "Upstairs Climate Control");

  finishQuickScreen(window);
  apply_actionbar_prototype(window, screen);
}

static void demo_show_quick_no_title(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

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
  Window *keeper = window_create();
  window_stack_push(keeper, false);

  app_timer_register(500, demo_show_quick_long, NULL);
  app_timer_register(25000, demo_show_quick_no_title, NULL);
  app_timer_register(60000, demo_show_list_long, NULL);
  app_timer_register(110000, demo_show_text_long, NULL);
  app_timer_register(160000, demo_show_list_custom, NULL);

  app_event_loop();

  if (prototypeActionBar) {
    action_bar_layer_destroy(prototypeActionBar);
  }
  if (prototypeUp) {
    gbitmap_destroy(prototypeUp);
  }
  if (prototypeSelect) {
    gbitmap_destroy(prototypeSelect);
  }
  if (prototypeDown) {
    gbitmap_destroy(prototypeDown);
  }
  window_destroy(keeper);
  return 0;
}
