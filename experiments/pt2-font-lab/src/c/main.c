#include <pebble.h>

typedef struct {
  const char *name;
  GFont font18;
  GFont font24;
  bool custom;
} Candidate;

static Window *s_window;
static TextLayer *s_name_layer;
static TextLayer *s_small_label;
static TextLayer *s_small_sample;
static TextLayer *s_large_label;
static TextLayer *s_large_sample;
static TextLayer *s_footer;

static Candidate s_candidates[5];
static int s_index;

static GFont s_atkinson18, s_atkinson24;
static GFont s_source18, s_source24;
static GFont s_fira18, s_fira24;
static GFont s_plex18, s_plex24;

static const char *SAMPLE_SMALL = "Playing, paging, glyphs\ngypqj QWMW 0123456789";
static const char *SAMPLE_LARGE = "Media Volume\nNothing Playing - 55%";

static void render_candidate(void) {
  Candidate *c = &s_candidates[s_index];
  text_layer_set_text(s_name_layer, c->name);
  text_layer_set_font(s_small_sample, c->font18);
  text_layer_set_font(s_large_sample, c->font24);
  text_layer_set_text(s_small_sample, SAMPLE_SMALL);
  text_layer_set_text(s_large_sample, SAMPLE_LARGE);
}

static void up_click(ClickRecognizerRef recognizer, void *context) {
  s_index = (s_index + 4) % 5;
  render_candidate();
}

static void down_click(ClickRecognizerRef recognizer, void *context) {
  s_index = (s_index + 1) % 5;
  render_candidate();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click);
}

static TextLayer *make_layer(GRect frame, GFont font, GTextAlignment alignment) {
  TextLayer *layer = text_layer_create(frame);
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_color(layer, GColorBlack);
  text_layer_set_font(layer, font);
  text_layer_set_text_alignment(layer, alignment);
  text_layer_set_overflow_mode(layer, GTextOverflowModeWordWrap);
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(layer));
  return layer;
}

static void window_load(Window *window) {
  Layer *root = window_get_root_layer(window);
  GRect b = layer_get_bounds(root);
  window_set_background_color(window, GColorWhite);

  s_name_layer = make_layer(GRect(4, 2, b.size.w - 8, 28),
      fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GTextAlignmentCenter);

  s_small_label = make_layer(GRect(4, 31, b.size.w - 8, 20),
      fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GTextAlignmentLeft);
  text_layer_set_text(s_small_label, "18 px");

  s_small_sample = make_layer(GRect(4, 49, b.size.w - 8, 62),
      fonts_get_system_font(FONT_KEY_GOTHIC_18), GTextAlignmentLeft);

  s_large_label = make_layer(GRect(4, 113, b.size.w - 8, 20),
      fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GTextAlignmentLeft);
  text_layer_set_text(s_large_label, "24 px");

  s_large_sample = make_layer(GRect(4, 131, b.size.w - 8, 70),
      fonts_get_system_font(FONT_KEY_GOTHIC_24), GTextAlignmentLeft);

  s_footer = make_layer(GRect(4, 204, b.size.w - 8, 20),
      fonts_get_system_font(FONT_KEY_GOTHIC_14), GTextAlignmentCenter);
  text_layer_set_text(s_footer, "UP / DOWN: change font");

  render_candidate();
}

static void window_unload(Window *window) {
  text_layer_destroy(s_name_layer);
  text_layer_destroy(s_small_label);
  text_layer_destroy(s_small_sample);
  text_layer_destroy(s_large_label);
  text_layer_destroy(s_large_sample);
  text_layer_destroy(s_footer);
}

static void init_fonts(void) {
  s_atkinson18 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ATKINSON_18));
  s_atkinson24 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ATKINSON_24));
  s_source18 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SOURCE_SANS_18));
  s_source24 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SOURCE_SANS_24));
  s_fira18 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FIRA_SANS_18));
  s_fira24 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FIRA_SANS_24));
  s_plex18 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IBM_PLEX_SANS_18));
  s_plex24 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IBM_PLEX_SANS_24));

  s_candidates[0] = (Candidate){"Pebble Raster Gothic",
      fonts_get_system_font(FONT_KEY_GOTHIC_18),
      fonts_get_system_font(FONT_KEY_GOTHIC_24), false};
  s_candidates[1] = (Candidate){"Atkinson Hyperlegible", s_atkinson18, s_atkinson24, true};
  s_candidates[2] = (Candidate){"Source Sans 3", s_source18, s_source24, true};
  s_candidates[3] = (Candidate){"Fira Sans", s_fira18, s_fira24, true};
  s_candidates[4] = (Candidate){"IBM Plex Sans", s_plex18, s_plex24, true};
}

static void deinit_fonts(void) {
  fonts_unload_custom_font(s_atkinson18);
  fonts_unload_custom_font(s_atkinson24);
  fonts_unload_custom_font(s_source18);
  fonts_unload_custom_font(s_source24);
  fonts_unload_custom_font(s_fira18);
  fonts_unload_custom_font(s_fira24);
  fonts_unload_custom_font(s_plex18);
  fonts_unload_custom_font(s_plex24);
}

static void init(void) {
  init_fonts();
  s_window = window_create();
  window_set_click_config_provider(s_window, click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers){
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(s_window, true);
}

static void deinit(void) {
  window_destroy(s_window);
  deinit_fonts();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
