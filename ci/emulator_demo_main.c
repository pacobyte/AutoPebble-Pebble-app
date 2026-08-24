#include "main_prod.inc"

/* AutoPebbleQuickScreen.c defines this provider but the legacy code does not
   expose it in a header. Keep the declaration local to the CI prototype. */
void config_quickscreen_click_provider(Window *window);

static ActionBarLayer *prototypeActionBar = NULL;
static GBitmap *prototypeUp = NULL;
static GBitmap *prototypeSelect = NULL;
static GBitmap *prototypeDown = NULL;

static const int emery_action_centers[3] = {54, 114, 174};
static const int emery_action_max_height = 58;

/* Prefer comfortably readable PT2 typography. Measure at the real content
   width and keep 28 Bold whenever the wrapped label fits inside its native
   button slot; fall back only to 24 Bold, never 18px for normal PT2 actions. */
static const char *prototype_font_for_label(const char *text, int width) {
  if (!text || !text[0]) {
    return FONT_KEY_GOTHIC_28_BOLD;
  }

  GSize large = graphics_text_layout_get_content_size(
      text,
      fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),
      GRect(0, 0, width, 120),
      GTextOverflowModeWordWrap,
      GTextAlignmentCenter);

  return large.h <= emery_action_max_height
      ? FONT_KEY_GOTHIC_28_BOLD
      : FONT_KEY_GOTHIC_24_BOLD;
}

static void prototype_click_config_provider(void *context) {
  config_quickscreen_click_provider((Window *)context);
}

static void apply_native_actionbar_prototype(AutoPebbleWindow *window,
                                             AutoPebbleQuickScreen *screen,
                                             bool show_title) {
  Layer *root = window_get_root_layer(window->window);
  GRect bounds = layer_get_bounds(root);
  const int content_width = bounds.size.w - ACTION_BAR_WIDTH;
  const int horizontal_padding = 4;
  const int label_width = content_width - (horizontal_padding * 2);

  /* Use Pebble's ActionBar exactly as intended: full-height and unmodified. */
  prototypeActionBar = action_bar_layer_create();
  prototypeUp = gbitmap_create_with_resource(RESOURCE_ID_ACTIONBAR_UP);
  prototypeSelect = gbitmap_create_with_resource(RESOURCE_ID_ACTIONBAR_SELECT);
  prototypeDown = gbitmap_create_with_resource(RESOURCE_ID_ACTIONBAR_DOWN);

  action_bar_layer_add_to_window(prototypeActionBar, window->window);
  action_bar_layer_set_context(prototypeActionBar, window->window);
  action_bar_layer_set_click_config_provider(prototypeActionBar,
                                              prototype_click_config_provider);
  action_bar_layer_set_icon_animated(prototypeActionBar, BUTTON_ID_UP,
                                     prototypeUp, true);
  action_bar_layer_set_icon_animated(prototypeActionBar, BUTTON_ID_SELECT,
                                     prototypeSelect, true);
  action_bar_layer_set_icon_animated(prototypeActionBar, BUTTON_ID_DOWN,
                                     prototypeDown, true);

  /* A supplied title is ordinary context content, never a separate bar. */
  text_layer_set_background_color(screen->textLayerTitle, GColorWhite);
  text_layer_set_text_color(screen->textLayerTitle, GColorBlack);
  if (show_title && screen->labelTitle && screen->labelTitle[0]) {
    setLayerText(screen->textLayerTitle, screen->labelTitle,
                 FONT_KEY_GOTHIC_18_BOLD);
    text_layer_set_text_alignment(screen->textLayerTitle, GTextAlignmentCenter);
    text_layer_set_overflow_mode(screen->textLayerTitle,
                                 GTextOverflowModeTrailingEllipsis);
    layer_set_frame(text_layer_get_layer(screen->textLayerTitle),
                    GRect(horizontal_padding, 0, label_width, 26));
  } else {
    text_layer_set_text(screen->textLayerTitle, "");
    layer_set_frame(text_layer_get_layer(screen->textLayerTitle),
                    GRect(0, 0, 0, 0));
  }

  TextLayer *layers[3] = {
    screen->textLayerTop,
    screen->textLayerMiddle,
    screen->textLayerBottom
  };
  char *labels[3] = {
    screen->labelTop,
    screen->labelMiddle,
    screen->labelBottom
  };

  for (int i = 0; i < 3; i++) {
    const char *font = window->textFont;
    if (!font) {
      font = prototype_font_for_label(labels[i], label_width - 4);
    }
    setLayerText(layers[i], labels[i], (char *)font);
    text_layer_set_text_alignment(layers[i], GTextAlignmentCenter);
    text_layer_set_overflow_mode(layers[i], GTextOverflowModeWordWrap);
    layer_set_clips(text_layer_get_layer(layers[i]), true);

    layer_set_frame(text_layer_get_layer(layers[i]),
                    GRect(horizontal_padding, 0, label_width,
                          emery_action_max_height));
    GSize content = text_layer_get_content_size(layers[i]);
    int height = content.h + 2;
    if (height > emery_action_max_height) {
      height = emery_action_max_height;
    }
    int y = emery_action_centers[i] - (height / 2);
    layer_set_frame(text_layer_get_layer(layers[i]),
                    GRect(horizontal_padding, y, label_width, height));
  }

  layer_mark_dirty(root);
}

static void demo_show_quick_with_title(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  screen->labelTitle = resetString(screen->labelTitle, "Control Hub");
  screen->labelTop = resetString(screen->labelTop, "Bedroom & Hallway Lights");
  screen->labelMiddle = resetString(screen->labelMiddle, "Open the Garage Door");
  screen->labelBottom = resetString(screen->labelBottom, "Upstairs Climate Control");

  finishQuickScreen(window);
  apply_native_actionbar_prototype(window, screen, true);
}

static void demo_show_quick_without_title(void *context) {
  AutoPebbleWindow *window = initQuickScreen();
  AutoPebbleQuickScreen *screen = getCurrentAutoPebbleQuickScreen();

  screen->labelTop = resetString(screen->labelTop, "Bedroom & Hallway Lights");
  screen->labelMiddle = resetString(screen->labelMiddle, "Open the Garage Door");
  screen->labelBottom = resetString(screen->labelBottom, "Upstairs Climate Control");

  finishQuickScreen(window);
  apply_native_actionbar_prototype(window, screen, false);
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
