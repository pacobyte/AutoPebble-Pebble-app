#include "AutoPebbleWindows.h"
#include "Utils.h"

#define TEXTSCREEN_TEXT  0
#define TEXTSCREEN_MIDDLE_ACTION  1
#define TEXTSCREEN_MIDDLE_LONG_ACTION  2
#define TEXTSCREEN_MIDDLE_MULTI_ACTION  3

static const int vert_scroll_text_padding = 4;
static const int initial_title_height = 22;

AutoPebbleTextScreen * newPebbleTextScreen(){
	struct AutoPebbleTextScreen * item = (AutoPebbleTextScreen*) malloc(sizeof(AutoPebbleTextScreen));
	item->labelTitle = NULL;
	item->labelText = NULL;
	item->textLayerTitle = NULL;
	item->textLayerText = NULL;
	item->scroll_layer = NULL;
	item->actionMiddle = NULL;
	item->longActionMiddle = NULL;
	item->multiActionMiddle = NULL;
	return item;
}

void freePebbleTextScreen(AutoPebbleTextScreen * item){
	free(item->labelTitle);
	item->labelTitle = NULL;
	free(item->labelText);
	item->labelText = NULL;

	text_layer_destroy(item->textLayerTitle);
	item->textLayerTitle = NULL;
	text_layer_destroy(item->textLayerText);
	item->textLayerText = NULL;

	scroll_layer_destroy(item->scroll_layer);
	item->scroll_layer = NULL;

	free(item->actionMiddle);
	item->actionMiddle = NULL;
	free(item->longActionMiddle);
	item->longActionMiddle = NULL;
	free(item->multiActionMiddle);
	item->multiActionMiddle = NULL;
}

AutoPebbleWindow * replaceWithAutoPebbleTextScreenWindow(Window * windowPebble,int id,bool bringToTop){
	return replaceWithAutoPebbleWindowFromType(windowPebble, PEBBLE_SCREEN_TYPE_TEXT_SCREEN,newPebbleTextScreen(),id,bringToTop);
}

AutoPebbleTextScreen* getAutoPebbleTextScreen(AutoPebbleWindow* window){
	return (AutoPebbleTextScreen*)getAutoPebbleItem(window);
}

AutoPebbleTextScreen* getCurrentAutoPebbleTextScreen(){
	return (AutoPebbleTextScreen*)getCurrentAutoPebbleItem();
}

AutoPebbleWindow * addAutoPebbleTextScreenWindow(Window* window){
	return addAutoPebbleWindowFromType(window, PEBBLE_SCREEN_TYPE_TEXT_SCREEN,newPebbleTextScreen());
}

void textScreenUnload(Window* win)
{
	APP_LOG(APP_LOG_LEVEL_INFO, "Destroying Text Screen Window.");
	removeAutoPebbleWindow(win);
	window_destroy(win);
}

void textscreen_middle_multi_click_handler(ClickRecognizerRef recognizer, void *context) {
	sendCommandActionToPhone(getCurrentAutoPebbleTextScreen()->multiActionMiddle);
}

void textscreen_middle_long_click_handler(ClickRecognizerRef recognizer, void *context) {
	sendCommandActionToPhone(getCurrentAutoPebbleTextScreen()->longActionMiddle);
}

void textscreen_middle_click_handler(ClickRecognizerRef recognizer, void *context) {
	sendCommandActionToPhone(getCurrentAutoPebbleTextScreen()->actionMiddle);
}

void textscreen_back_multi_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	bool sent = sendCommandActionToPhone(window->multiActionBack);
	if(!sent){
		window_stack_pop(true);
	}else{
		setShouldPopWindowAfterCommand(window->doBackOnLongBackCommand);
	}
}

void textscreen_back_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	bool sent = sendCommandActionToPhone(window->actionBack);
	if(!sent){
		window_stack_pop(true);
	}else{
		setShouldPopWindowAfterCommand(window->doBackOnBackCommand);
	}
}

void config_textscreen_click_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, textscreen_middle_click_handler);
	window_long_click_subscribe(BUTTON_ID_SELECT, 0, textscreen_middle_long_click_handler, NULL);
	window_multi_click_subscribe(BUTTON_ID_SELECT, 2, 0, 0, true, textscreen_middle_multi_click_handler);
	window_single_click_subscribe(BUTTON_ID_BACK, textscreen_back_click_handler);
	window_multi_click_subscribe(BUTTON_ID_BACK, 2, 0, 0, true, textscreen_back_multi_click_handler);
}

static int text_screen_horizontal_padding(GRect bounds){
	return bounds.size.w >= 180 ? 4 : 0;
}

static int configure_title_layer(TextLayer * titleLayer, const char * title, const char * titleFont, GRect bounds){
	if(!title || !title[0]){
		return 0;
	}

	text_layer_set_text_alignment(titleLayer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(titleLayer, GTextOverflowModeTrailingEllipsis);
	text_layer_set_text(titleLayer, title);
	text_layer_set_font(titleLayer, fonts_get_system_font(titleFont));

	GSize titleSize = text_layer_get_content_size(titleLayer);
	int titleHeight = titleSize.h + 3;

	/* Keep pathological titles from consuming most of the PT2 display. */
	if(bounds.size.w >= 180 && titleHeight > 48){
		titleHeight = 48;
	}

	layer_set_frame(text_layer_get_layer(titleLayer), GRect(0, 0, bounds.size.w, titleHeight));
	return titleHeight;
}

static void configure_text_body(AutoPebbleTextScreen * screen, const char * text, const char * textFont, GRect bounds){
	int horizontalPadding = text_screen_horizontal_padding(bounds);
	int textWidth = bounds.size.w - (horizontalPadding * 2);

	screen->textLayerText = text_layer_create(GRect(horizontalPadding, vert_scroll_text_padding, textWidth, 2000));
	setLayerText(screen->textLayerText, (char *)(text ? text : ""), (char *)textFont);
	scroll_layer_add_child(screen->scroll_layer, text_layer_get_layer(screen->textLayerText));

	#ifdef PBL_ROUND
		text_layer_set_text_alignment(screen->textLayerText, GTextAlignmentCenter);
		text_layer_enable_screen_text_flow_and_paging(screen->textLayerText, 4);
	#else
		/* On PT2/Emery, use Pebble's pagination-aware text layout so page
		   boundaries do not cut through glyphs when Up/Down is pressed. */
		if(bounds.size.w >= 180){
			text_layer_enable_screen_text_flow_and_paging(screen->textLayerText, 0);
		}
	#endif

	GSize bodySize = text_layer_get_content_size(screen->textLayerText);
	bodySize.h += 8;
	text_layer_set_size(screen->textLayerText, GSize(textWidth, bodySize.h));

	scroll_layer_set_content_size(screen->scroll_layer,
		GSize(bounds.size.w, bodySize.h + (vert_scroll_text_padding * 2)));
}

AutoPebbleWindow * initTextScreen(){
	APP_LOG(APP_LOG_LEVEL_INFO, "INIT TEXT SCREEN -----------------------------/////");

	const char* locale_str = i18n_get_system_locale();
	WindowHandlers wh = { .unload = &textScreenUnload };
	Window * window = window_create();
	window_set_window_handlers(window, wh);
	AutoPebbleWindow * autoPebbleWindow = addAutoPebbleTextScreenWindow(window);
	AutoPebbleTextScreen* autoPebbleTextScreen = getAutoPebbleTextScreen(autoPebbleWindow);

	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	/* The title is fixed. Only the body belongs to the ScrollLayer. */
	autoPebbleTextScreen->textLayerTitle = text_layer_create(GRect(0, 0, bounds.size.w, initial_title_height));
	text_layer_set_text_alignment(autoPebbleTextScreen->textLayerTitle, GTextAlignmentCenter);
	text_layer_set_font(autoPebbleTextScreen->textLayerTitle, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_text(autoPebbleTextScreen->textLayerTitle, "AutoPebble");
	layer_add_child(window_layer, text_layer_get_layer(autoPebbleTextScreen->textLayerTitle));

	GRect scrollBounds = GRect(0, initial_title_height, bounds.size.w, bounds.size.h - initial_title_height);
	autoPebbleTextScreen->scroll_layer = scroll_layer_create(scrollBounds);

	ScrollLayerCallbacks scrollCallbacks = {.click_config_provider = &config_textscreen_click_provider};
	scroll_layer_set_callbacks(autoPebbleTextScreen->scroll_layer, scrollCallbacks);
	scroll_layer_set_click_config_onto_window(autoPebbleTextScreen->scroll_layer, window);
	#ifdef PBL_ROUND
		scroll_layer_set_paging(autoPebbleTextScreen->scroll_layer, true);
	#else
		if(bounds.size.w >= 180){
			scroll_layer_set_paging(autoPebbleTextScreen->scroll_layer, true);
		}
	#endif

	const char * loadingText;
	if (strncmp(locale_str, "ru", 2) == 0){
		loadingText = "Загрузка Text Screen...";
	}else{
		loadingText = "Loading Text Screen...";
	}
	configure_text_body(autoPebbleTextScreen, loadingText, FONT_KEY_GOTHIC_18, bounds);

	layer_add_child(window_layer, scroll_layer_get_layer(autoPebbleTextScreen->scroll_layer));
	/* Keep the fixed title above the scrolling body in z-order. */
	layer_add_child(window_layer, text_layer_get_layer(autoPebbleTextScreen->textLayerTitle));

	window_stack_push(window, true);
	return autoPebbleWindow;
}

void handleTextScreen(DictionaryIterator *received, void *context, AutoPebbleWindow* window){
	AutoPebbleTextScreen* autoPebbleTextScreen = getAutoPebbleTextScreen(window);
	Tuple *tuple = dict_read_first(received);
	while (tuple) {
		if(tuple->key == PEBBLE_COMMAND_HEADER){
			autoPebbleTextScreen->labelTitle = setString(autoPebbleTextScreen->labelTitle, tuple->value->cstring);
		}else if(tuple->key == TEXTSCREEN_TEXT){
			autoPebbleTextScreen->labelText = setString(autoPebbleTextScreen->labelText, tuple->value->cstring);
		}else if(tuple->key == TEXTSCREEN_MIDDLE_ACTION){
			autoPebbleTextScreen->actionMiddle = resetString(autoPebbleTextScreen->actionMiddle, tuple->value->cstring);
		}else if(tuple->key == TEXTSCREEN_MIDDLE_LONG_ACTION){
			autoPebbleTextScreen->longActionMiddle = resetString(autoPebbleTextScreen->longActionMiddle, tuple->value->cstring);
		}else if(tuple->key == TEXTSCREEN_MIDDLE_MULTI_ACTION){
			autoPebbleTextScreen->multiActionMiddle = resetString(autoPebbleTextScreen->multiActionMiddle, tuple->value->cstring);
		}
		tuple = dict_read_next(received);
	}
}

void finishTextScreen(AutoPebbleWindow * window){
	APP_LOG(APP_LOG_LEVEL_INFO, "FINISH TEXT SCREEN -----------------------------/////");
	AutoPebbleTextScreen* autoPebbleTextScreen = getAutoPebbleTextScreen(window);

	char * textFont = window->textFont;
	if(!textFont){
		textFont = FONT_KEY_GOTHIC_18;
	}
	char * titleFont = window->titleFont;
	if(!titleFont){
		titleFont = FONT_KEY_GOTHIC_18_BOLD;
	}

	text_layer_destroy(autoPebbleTextScreen->textLayerTitle);
	autoPebbleTextScreen->textLayerTitle = NULL;
	text_layer_destroy(autoPebbleTextScreen->textLayerText);
	autoPebbleTextScreen->textLayerText = NULL;

	Layer *window_layer = window_get_root_layer(window->window);
	GRect bounds = layer_get_bounds(window_layer);

	int titleHeight = 0;
	if(autoPebbleTextScreen->labelTitle && autoPebbleTextScreen->labelTitle[0]){
		autoPebbleTextScreen->textLayerTitle = text_layer_create(GRect(0, 0, bounds.size.w, 200));
		titleHeight = configure_title_layer(autoPebbleTextScreen->textLayerTitle,
			autoPebbleTextScreen->labelTitle, titleFont, bounds);
		layer_add_child(window_layer, text_layer_get_layer(autoPebbleTextScreen->textLayerTitle));
	}

	GRect scrollBounds = GRect(0, titleHeight, bounds.size.w, bounds.size.h - titleHeight);
	layer_set_frame(scroll_layer_get_layer(autoPebbleTextScreen->scroll_layer), scrollBounds);

	#ifdef PBL_ROUND
		scroll_layer_set_paging(autoPebbleTextScreen->scroll_layer, true);
	#else
		scroll_layer_set_paging(autoPebbleTextScreen->scroll_layer, bounds.size.w >= 180);
	#endif

	configure_text_body(autoPebbleTextScreen, autoPebbleTextScreen->labelText, textFont, bounds);

	/* Re-add the fixed title last so it remains visually above the scroll viewport. */
	if(autoPebbleTextScreen->textLayerTitle){
		layer_add_child(window_layer, text_layer_get_layer(autoPebbleTextScreen->textLayerTitle));
	}

	layer_mark_dirty(window_layer);
}
