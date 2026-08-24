
#include "AutoPebbleWindows.h"
#include "Utils.h"

#define QUICKSCREEN_TOP_LABEL  0
#define QUICKSCREEN_TOP_ACTION  1
#define QUICKSCREEN_TOP_LONG_ACTION  2
#define QUICKSCREEN_MIDDLE_LABEL  3
#define QUICKSCREEN_MIDDLE_ACTION  4
#define QUICKSCREEN_MIDDLE_LONG_ACTION  5
#define QUICKSCREEN_BOTTOM_LABEL  6
#define QUICKSCREEN_BOTTOM_ACTION  7
#define QUICKSCREEN_BOTTOM_LONG_ACTION  8
#define QUICKSCREEN_TOP_MULTI_ACTION  9
#define QUICKSCREEN_MIDDLE_MULTI_ACTION  10
#define QUICKSCREEN_BOTTOM_MULTI_ACTION  11
#define QUICKSCREEN_ACTION_PREFIX  12
#define QUICKSCREEN_LONG_ACTION_PREFIX  13
#define QUICKSCREEN_MULTI_ACTION_PREFIX  14

#define QUICKSCREEN_PT2_ACTION_HEIGHT 58

AutoPebbleQuickScreen * newPebbleQuickScreen(){
	struct AutoPebbleQuickScreen * item = (AutoPebbleQuickScreen*) malloc(sizeof(AutoPebbleQuickScreen));
	item->labelTop = NULL;
	item->labelMiddle = NULL;
	item->labelBottom = NULL;
	item->labelTitle = NULL;

	item->actionTop = NULL;
	item->actionMiddle = NULL;
	item->actionBottom = NULL;

	item->longActionTop = NULL;
	item->longActionMiddle = NULL;
	item->longActionBottom = NULL;

	item->multiActionTop = NULL;
	item->multiActionMiddle = NULL;
	item->multiActionBottom = NULL;

	item->commandPrefix = NULL;
	item->longCommandPrefix = NULL;
	item->multiCommandPrefix = NULL;
	item->textLayerTop = NULL;
	item->textLayerMiddle = NULL;
	item->textLayerBottom = NULL;
	item->textLayerTitle = NULL;
	item->actionBar = NULL;
	item->actionBarUp = NULL;
	item->actionBarSelect = NULL;
	item->actionBarDown = NULL;
	return item;
}
void freePebbleQuickScreen(AutoPebbleQuickScreen * item){
	free(item->labelTop);
	item->labelTop = NULL;
	free(item->labelMiddle);
	item->labelMiddle = NULL;
	free(item->labelBottom);
	item->labelBottom = NULL;
	free(item->labelTitle);
	item->labelTitle = NULL;

	free(item->actionTop);
	item->actionTop = NULL;
	free(item->actionMiddle);
	item->actionMiddle = NULL;
	free(item->actionBottom);
	item->actionBottom = NULL;

	free(item->longActionTop);
	item->longActionTop = NULL;
	free(item->longActionMiddle);
	item->longActionMiddle = NULL;
	free(item->longActionBottom);
	item->longActionBottom = NULL;

	free(item->multiActionTop);
	item->multiActionTop = NULL;
	free(item->multiActionMiddle);
	item->multiActionMiddle = NULL;
	free(item->multiActionBottom);
	item->multiActionBottom = NULL;

	free(item->commandPrefix);
	item->commandPrefix = NULL;

	free(item->longCommandPrefix);
	item->longCommandPrefix = NULL;

	free(item->multiCommandPrefix);
	item->multiCommandPrefix = NULL;

	/* ActionBar retains the bitmap pointers, so destroy it first. */
	if(item->actionBar){
		action_bar_layer_destroy(item->actionBar);
		item->actionBar = NULL;
	}
	if(item->actionBarUp){
		gbitmap_destroy(item->actionBarUp);
		item->actionBarUp = NULL;
	}
	if(item->actionBarSelect){
		gbitmap_destroy(item->actionBarSelect);
		item->actionBarSelect = NULL;
	}
	if(item->actionBarDown){
		gbitmap_destroy(item->actionBarDown);
		item->actionBarDown = NULL;
	}

	text_layer_destroy(item->textLayerTop);
	item->textLayerTop = NULL;
	text_layer_destroy(item->textLayerMiddle);
	item->textLayerMiddle = NULL;
	text_layer_destroy(item->textLayerBottom);
	item->textLayerBottom = NULL;
	if(item->textLayerTitle){
		text_layer_destroy(item->textLayerTitle);
	}
	item->textLayerTitle = NULL;
}

AutoPebbleQuickScreen* getAutoPebbleQuickScreen(AutoPebbleWindow* window){
	return (AutoPebbleQuickScreen*)getAutoPebbleItem(window);
}
AutoPebbleQuickScreen* getCurrentAutoPebbleQuickScreen(){
	return (AutoPebbleQuickScreen*)getCurrentAutoPebbleItem();
}
AutoPebbleWindow * replaceWithAutoPebbleQuickScreenWindow(Window * windowPebble, int id,bool bringToTop){
	return replaceWithAutoPebbleWindowFromType(windowPebble, PEBBLE_SCREEN_TYPE_QUICK_SCREEN,newPebbleQuickScreen(),id,bringToTop);
}
AutoPebbleWindow * addAutoPebbleQuickScreenWindow(Window* window){
	return addAutoPebbleWindowFromType(window, PEBBLE_SCREEN_TYPE_QUICK_SCREEN,newPebbleQuickScreen());
}
void quickScreenUnload(Window* win)
{
	APP_LOG(APP_LOG_LEVEL_INFO, "Destroying Quick Screen Window.");
	removeAutoPebbleWindow(win);
	window_destroy(win);
}
;
void deselectOption(void *data){
	TextLayer * textLayer = (TextLayer * )data;
	text_layer_set_text_color(textLayer, GColorBlack);
	text_layer_set_background_color(textLayer,GColorWhite );
}

void doSelectOptionAnimation(TextLayer * textLayer){
	AutoPebbleQuickScreen * quickScreen = getCurrentAutoPebbleQuickScreen();
	/* PT2 uses Pebble's native ActionBar press animation instead. */
	if(quickScreen && quickScreen->actionBar){
		return;
	}
	if(text_layer_get_text(textLayer)){
		#ifdef PBL_COLOR
			text_layer_set_text_color(textLayer, GColorBlack);
			text_layer_set_background_color(textLayer, GColorVividCerulean);
		#else
			text_layer_set_text_color(textLayer, GColorWhite);
			text_layer_set_background_color(textLayer, GColorBlack);
		#endif

		app_timer_register(150,deselectOption,textLayer);
	}
}
char * getCommandPrefix(AutoPebbleQuickScreen * quickScreen, char * defaultPrefix){
	if(defaultPrefix){
		return defaultPrefix;
	}else{
		return quickScreen->commandPrefix;
	}
}
void quickscreen_up_multi_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	AutoPebbleQuickScreen * quickScreen = getAutoPebbleQuickScreen(window);
	sendActionWithPrefixAndFallbackToPhone(window, getCommandPrefix(quickScreen, quickScreen->multiCommandPrefix), quickScreen->multiActionTop,quickScreen->labelTop);
	doSelectOptionAnimation(quickScreen->textLayerTop);
}
void quickscreen_middle_multi_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	AutoPebbleQuickScreen * quickScreen = getAutoPebbleQuickScreen(window);
	sendActionWithPrefixAndFallbackToPhone(window, getCommandPrefix(quickScreen,quickScreen->multiCommandPrefix), quickScreen->multiActionMiddle,quickScreen->labelMiddle);
	doSelectOptionAnimation(quickScreen->textLayerMiddle);
}
void quickscreen_down_multi_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	AutoPebbleQuickScreen * quickScreen = getAutoPebbleQuickScreen(window);
	sendActionWithPrefixAndFallbackToPhone(window, getCommandPrefix(quickScreen,quickScreen->multiCommandPrefix), quickScreen->multiActionBottom,quickScreen->labelBottom);
	doSelectOptionAnimation(quickScreen->textLayerBottom);
}
void quickscreen_up_long_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	AutoPebbleQuickScreen * quickScreen = getAutoPebbleQuickScreen(window);
	sendActionWithPrefixAndFallbackToPhone(window, quickScreen->longCommandPrefix, quickScreen->longActionTop,quickScreen->labelTop);
	doSelectOptionAnimation(quickScreen->textLayerTop);
}
void quickscreen_middle_long_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	AutoPebbleQuickScreen * quickScreen = getAutoPebbleQuickScreen(window);
	sendActionWithPrefixAndFallbackToPhone(window, getCommandPrefix(quickScreen,quickScreen->longCommandPrefix),quickScreen->longActionMiddle,quickScreen->labelMiddle);
	doSelectOptionAnimation(quickScreen->textLayerMiddle);
}
void quickscreen_down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	AutoPebbleQuickScreen * quickScreen = getAutoPebbleQuickScreen(window);
	sendActionWithPrefixAndFallbackToPhone(window, getCommandPrefix(quickScreen,quickScreen->longCommandPrefix),quickScreen->longActionBottom,quickScreen->labelBottom);
	doSelectOptionAnimation(quickScreen->textLayerBottom);
}
void quickscreen_up_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	AutoPebbleQuickScreen * quickScreen = getAutoPebbleQuickScreen(window);

	sendActionWithPrefixAndFallbackToPhone(window, quickScreen->commandPrefix, quickScreen->actionTop,quickScreen->labelTop);
	doSelectOptionAnimation(quickScreen->textLayerTop);
}
void quickscreen_middle_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	AutoPebbleQuickScreen * quickScreen = getAutoPebbleQuickScreen(window);
	sendActionWithPrefixAndFallbackToPhone(window, quickScreen->commandPrefix, quickScreen->actionMiddle,quickScreen->labelMiddle);
	doSelectOptionAnimation(quickScreen->textLayerMiddle);
}
void quickscreen_down_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	AutoPebbleQuickScreen * quickScreen = getAutoPebbleQuickScreen(window);
	sendActionWithPrefixAndFallbackToPhone(window, quickScreen->commandPrefix, quickScreen->actionBottom,quickScreen->labelBottom);

	doSelectOptionAnimation(quickScreen->textLayerBottom);
}
void quickscreen_back_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	AutoPebbleQuickScreen * quickScreen = getAutoPebbleQuickScreen(window);
	char * prefix = quickScreen->commandPrefix;
	char * action = window->actionBack;
	if(window->noPrefixIfCommand){
		logString("no prefix");
		if(contains(action, "=:=")){
			prefix = NULL;
		}
	}
	bool sent = sendActionWithPrefixToPhone(prefix,action);
	if(!sent){
		window_stack_pop(true);
	}else{
		setShouldPopWindowAfterCommand(window->doBackOnBackCommand);
	}
}
void quickscreen_back_multi_click_handler(ClickRecognizerRef recognizer, void *context) {
	AutoPebbleWindow * window = getCurrentAutoPebbleWindow();
	AutoPebbleQuickScreen * quickScreen = getAutoPebbleQuickScreen(window);
	char * prefix = quickScreen->multiCommandPrefix;
	char * action = window->multiActionBack;
	if(window->noPrefixIfCommand){
		logString("no prefix");
		if(contains(action, "=:=")){
			prefix = NULL;
		}
	}
	bool sent = sendActionWithPrefixToPhone(prefix,action);
	if(!sent){
		window_stack_pop(true);
	}else{
		setShouldPopWindowAfterCommand(window->doBackOnLongBackCommand);
	}
}
void config_quickscreen_click_provider(Window *window) {
	window_single_click_subscribe(BUTTON_ID_UP, quickscreen_up_click_handler);
	window_single_click_subscribe(BUTTON_ID_SELECT, quickscreen_middle_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, quickscreen_down_click_handler);
	window_single_click_subscribe(BUTTON_ID_BACK, quickscreen_back_click_handler);

	window_long_click_subscribe(BUTTON_ID_UP,0, quickscreen_up_long_click_handler, NULL);
	window_long_click_subscribe(BUTTON_ID_SELECT,0, quickscreen_middle_long_click_handler, NULL);
	window_long_click_subscribe(BUTTON_ID_DOWN,0, quickscreen_down_long_click_handler, NULL);

	window_multi_click_subscribe(BUTTON_ID_UP,2, 0, 0, true, quickscreen_up_multi_click_handler);
	window_multi_click_subscribe(BUTTON_ID_SELECT,2, 0, 0, true, quickscreen_middle_multi_click_handler);
	window_multi_click_subscribe(BUTTON_ID_DOWN,2, 0, 0, true, quickscreen_down_multi_click_handler);
	window_multi_click_subscribe(BUTTON_ID_BACK,2, 0, 0, true, quickscreen_back_multi_click_handler);
}

static void quickscreen_actionbar_click_config_provider(void *context){
	config_quickscreen_click_provider((Window *)context);
}

static const char * quickscreen_default_font_for_text(const char * text, GRect bounds){
	if(bounds.size.w < 180){
		return FONT_KEY_GOTHIC_18_BOLD;
	}
	if(!text || !text[0]){
		return FONT_KEY_GOTHIC_24_BOLD;
	}

	GSize largeSize = graphics_text_layout_get_content_size(
		text,
		fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
		GRect(0, 0, 1000, 60),
		GTextOverflowModeWordWrap,
		GTextAlignmentCenter);
	return largeSize.w <= (bounds.size.w - 12) ? FONT_KEY_GOTHIC_24_BOLD : FONT_KEY_GOTHIC_18_BOLD;
}

static const char * quickscreen_pt2_font_for_text(const char * text, int width){
	if(!text || !text[0]){
		return FONT_KEY_GOTHIC_28_BOLD;
	}

	GSize largeSize = graphics_text_layout_get_content_size(
		text,
		fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),
		GRect(0, 0, width, 120),
		GTextOverflowModeWordWrap,
		GTextAlignmentCenter);
	return largeSize.h <= QUICKSCREEN_PT2_ACTION_HEIGHT ? FONT_KEY_GOTHIC_28_BOLD : FONT_KEY_GOTHIC_24_BOLD;
}

static void quickscreen_prepare_label(TextLayer * layer, char * text, char * explicitFont, GRect bounds){
	char * font = explicitFont;
	if(!font){
		font = (char *)quickscreen_default_font_for_text(text, bounds);
	}
	setLayerText(layer, text, font);
	text_layer_set_text_alignment(layer, GTextAlignmentCenter);
	if(bounds.size.w >= 180){
		text_layer_set_overflow_mode(layer, GTextOverflowModeWordWrap);
		layer_set_clips(text_layer_get_layer(layer), true);
	}
}

AutoPebbleWindow * initQuickScreen(){
	const char* locale_str = i18n_get_system_locale();
	WindowHandlers wh = { .unload = &quickScreenUnload };
	Window * window = window_create();
	window_set_window_handlers(window, wh);
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);
	const char * defaultTextFont = bounds.size.w >= 180 ? FONT_KEY_GOTHIC_28_BOLD : FONT_KEY_GOTHIC_18_BOLD;

	AutoPebbleWindow* autoPebbleWindow = addAutoPebbleQuickScreenWindow(window);
	AutoPebbleQuickScreen* autoPebbleQuickScreen = getAutoPebbleQuickScreen(autoPebbleWindow);

	TextLayer* textLayerTitle = text_layer_create(GRect(0,0,bounds.size.w,30));
	text_layer_set_text_alignment(textLayerTitle, GTextAlignmentCenter);
	text_layer_set_font(textLayerTitle, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text(textLayerTitle, "AutoPebble");
	#ifdef PBL_COLOR
		text_layer_set_text_color(textLayerTitle, GColorBlack);
		text_layer_set_background_color(textLayerTitle, GColorVividCerulean);
	#else
		text_layer_set_text_color(textLayerTitle, GColorWhite);
		text_layer_set_background_color(textLayerTitle, GColorBlack);
	#endif
	autoPebbleQuickScreen->textLayerTitle = textLayerTitle;
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerTitle));

	TextLayer* textLayerTop = text_layer_create(GRect(0,35,bounds.size.w,30));
	text_layer_set_text_alignment(textLayerTop, GTextAlignmentCenter);
	text_layer_set_font(textLayerTop, fonts_get_system_font(defaultTextFont));
	autoPebbleQuickScreen->textLayerTop = textLayerTop;
	layer_set_clips(text_layer_get_layer(autoPebbleQuickScreen->textLayerTop), false);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerTop));

	TextLayer* textLayerMiddle = text_layer_create(GRect(0,76,bounds.size.w,30));
	text_layer_set_text_alignment(textLayerMiddle, GTextAlignmentCenter);
	text_layer_set_font(textLayerMiddle, fonts_get_system_font(defaultTextFont));
	if (strncmp(locale_str, "ru", 2) == 0){
		text_layer_set_text(textLayerMiddle, "Загрузка Quick Screen...");
	}else{
		text_layer_set_text(textLayerMiddle, "Loading Quick Screen...");
	}
	autoPebbleQuickScreen->textLayerMiddle = textLayerMiddle;
	layer_set_clips(text_layer_get_layer(autoPebbleQuickScreen->textLayerMiddle), false);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerMiddle));

	TextLayer* textLayerBottom = text_layer_create(GRect(0,133,bounds.size.w,30));
	text_layer_set_text_alignment(textLayerBottom, GTextAlignmentCenter);
	text_layer_set_font(textLayerBottom, fonts_get_system_font(defaultTextFont));
	autoPebbleQuickScreen->textLayerBottom = textLayerBottom;
	layer_set_clips(text_layer_get_layer(autoPebbleQuickScreen->textLayerBottom), false);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayerBottom));

	window_set_click_config_provider(window, (ClickConfigProvider) config_quickscreen_click_provider);

	window_stack_push(window, true);
	return autoPebbleWindow;
}
void handleQuickScreen(DictionaryIterator *received, void *context, AutoPebbleWindow* window){
	AutoPebbleQuickScreen* autoPebbleQuickScreen = getAutoPebbleQuickScreen(window);
	Tuple *tuple = dict_read_first(received);
	while (tuple) {
		if(tuple->key == QUICKSCREEN_TOP_LABEL){
			autoPebbleQuickScreen->labelTop = resetString(autoPebbleQuickScreen->labelTop, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_TOP_ACTION){
			autoPebbleQuickScreen->actionTop = resetString(autoPebbleQuickScreen->actionTop, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_TOP_LONG_ACTION){
			autoPebbleQuickScreen->longActionTop = resetString(autoPebbleQuickScreen->longActionTop, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_TOP_MULTI_ACTION){
			autoPebbleQuickScreen->multiActionTop = resetString(autoPebbleQuickScreen->multiActionTop, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_MIDDLE_LABEL){
			autoPebbleQuickScreen->labelMiddle = resetString(autoPebbleQuickScreen->labelMiddle, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_MIDDLE_ACTION){
			autoPebbleQuickScreen->actionMiddle = resetString(autoPebbleQuickScreen->actionMiddle, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_MIDDLE_LONG_ACTION){
			autoPebbleQuickScreen->longActionMiddle = resetString(autoPebbleQuickScreen->longActionMiddle, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_MIDDLE_MULTI_ACTION){
			autoPebbleQuickScreen->multiActionMiddle = resetString(autoPebbleQuickScreen->multiActionMiddle, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_BOTTOM_LABEL){
			autoPebbleQuickScreen->labelBottom = resetString(autoPebbleQuickScreen->labelBottom, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_BOTTOM_ACTION){
			autoPebbleQuickScreen->actionBottom = resetString(autoPebbleQuickScreen->actionBottom, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_BOTTOM_LONG_ACTION){
			autoPebbleQuickScreen->longActionBottom = resetString(autoPebbleQuickScreen->longActionBottom, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_BOTTOM_MULTI_ACTION){
			autoPebbleQuickScreen->multiActionBottom = resetString(autoPebbleQuickScreen->multiActionBottom, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_ACTION_PREFIX){
			autoPebbleQuickScreen->commandPrefix = resetString(autoPebbleQuickScreen->commandPrefix, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_LONG_ACTION_PREFIX){
			autoPebbleQuickScreen->longCommandPrefix = resetString(autoPebbleQuickScreen->longCommandPrefix, tuple->value->cstring);
		}else if(tuple->key == QUICKSCREEN_MULTI_ACTION_PREFIX){
			autoPebbleQuickScreen->multiCommandPrefix = resetString(autoPebbleQuickScreen->multiCommandPrefix, tuple->value->cstring);
		}else if(tuple->key == PEBBLE_COMMAND_HEADER){
			autoPebbleQuickScreen->labelTitle = resetString(autoPebbleQuickScreen->labelTitle, tuple->value->cstring);
		}
		tuple = dict_read_next(received);
	}
}

static void finishQuickScreenPT2(AutoPebbleWindow * window, AutoPebbleQuickScreen * screen, GRect bounds){
	const int actionCenters[3] = {54, 114, 174};
	const int contentWidth = bounds.size.w - ACTION_BAR_WIDTH;
	const int horizontalPadding = 4;
	const int labelWidth = contentWidth - (horizontalPadding * 2);

	if(!screen->actionBar){
		screen->actionBar = action_bar_layer_create();
		screen->actionBarUp = gbitmap_create_with_resource(RESOURCE_ID_ACTIONBAR_UP);
		screen->actionBarSelect = gbitmap_create_with_resource(RESOURCE_ID_ACTIONBAR_SELECT);
		screen->actionBarDown = gbitmap_create_with_resource(RESOURCE_ID_ACTIONBAR_DOWN);

		action_bar_layer_add_to_window(screen->actionBar, window->window);
		action_bar_layer_set_context(screen->actionBar, window->window);
		action_bar_layer_set_click_config_provider(screen->actionBar, quickscreen_actionbar_click_config_provider);
		action_bar_layer_set_icon_animated(screen->actionBar, BUTTON_ID_UP, screen->actionBarUp, true);
		action_bar_layer_set_icon_animated(screen->actionBar, BUTTON_ID_SELECT, screen->actionBarSelect, true);
		action_bar_layer_set_icon_animated(screen->actionBar, BUTTON_ID_DOWN, screen->actionBarDown, true);
	}

	/* PT2 treats a supplied title as optional context, not separate chrome. */
	text_layer_set_background_color(screen->textLayerTitle, GColorWhite);
	text_layer_set_text_color(screen->textLayerTitle, GColorBlack);
	if(screen->labelTitle && screen->labelTitle[0]){
		char * titleFont = window->titleFont ? window->titleFont : FONT_KEY_GOTHIC_18_BOLD;
		setLayerText(screen->textLayerTitle, screen->labelTitle, titleFont);
		text_layer_set_text_alignment(screen->textLayerTitle, GTextAlignmentCenter);
		text_layer_set_overflow_mode(screen->textLayerTitle, GTextOverflowModeTrailingEllipsis);
		layer_set_frame(text_layer_get_layer(screen->textLayerTitle), GRect(horizontalPadding, 0, labelWidth, 26));
	}else{
		text_layer_set_text(screen->textLayerTitle, "");
		layer_set_frame(text_layer_get_layer(screen->textLayerTitle), GRect(0, 0, 0, 0));
	}

	TextLayer * layers[3] = {
		screen->textLayerTop,
		screen->textLayerMiddle,
		screen->textLayerBottom
	};
	char * labels[3] = {
		screen->labelTop,
		screen->labelMiddle,
		screen->labelBottom
	};

	for(int i = 0; i < 3; i++){
		char * font = window->textFont;
		if(!font){
			font = (char *)quickscreen_pt2_font_for_text(labels[i], labelWidth - 4);
		}
		setLayerText(layers[i], labels[i], font);
		text_layer_set_text_alignment(layers[i], GTextAlignmentCenter);
		text_layer_set_overflow_mode(layers[i], GTextOverflowModeWordWrap);
		layer_set_clips(text_layer_get_layer(layers[i]), true);

		layer_set_frame(text_layer_get_layer(layers[i]), GRect(horizontalPadding, 0, labelWidth, QUICKSCREEN_PT2_ACTION_HEIGHT));
		GSize labelSize = text_layer_get_content_size(layers[i]);
		int layerHeight = labelSize.h + 2;
		if(layerHeight > QUICKSCREEN_PT2_ACTION_HEIGHT){
			layerHeight = QUICKSCREEN_PT2_ACTION_HEIGHT;
		}
		int y = actionCenters[i] - (layerHeight / 2);
		layer_set_frame(text_layer_get_layer(layers[i]), GRect(horizontalPadding, y, labelWidth, layerHeight));
	}

	layer_mark_dirty(window_get_root_layer(window->window));
}

void finishQuickScreen(AutoPebbleWindow * window){
	AutoPebbleQuickScreen* autoPebbleQuickScreen = getAutoPebbleQuickScreen(window);
	Layer *window_layer = window_get_root_layer(window->window);
	GRect bounds = layer_get_frame(window_layer);
	bool isWide = bounds.size.w >= 180;

	if(isWide){
		finishQuickScreenPT2(window, autoPebbleQuickScreen, bounds);
		return;
	}

	char * titleFont = window->titleFont;
	if(!titleFont){
		titleFont = (char *)quickscreen_default_font_for_text(autoPebbleQuickScreen->labelTitle, bounds);
	}

	int titleBarHeight = 0;
	if(autoPebbleQuickScreen->labelTitle && autoPebbleQuickScreen->labelTitle[0]){
		setLayerText(autoPebbleQuickScreen->textLayerTitle, autoPebbleQuickScreen->labelTitle, titleFont);
		text_layer_set_text_alignment(autoPebbleQuickScreen->textLayerTitle, GTextAlignmentCenter);
		layer_set_frame(text_layer_get_layer(autoPebbleQuickScreen->textLayerTitle), GRect(0,0,bounds.size.w,40));
		GSize sizeTitle = text_layer_get_content_size(autoPebbleQuickScreen->textLayerTitle);
		titleBarHeight = sizeTitle.h + 8;
		layer_set_frame(text_layer_get_layer(autoPebbleQuickScreen->textLayerTitle), GRect(0,0,bounds.size.w,titleBarHeight));
	}else{
		/* Keep the layer alive so later refreshes can safely add a title. */
		text_layer_set_text(autoPebbleQuickScreen->textLayerTitle, "");
		layer_set_frame(text_layer_get_layer(autoPebbleQuickScreen->textLayerTitle), GRect(0,0,bounds.size.w,0));
	}

	int statusBarHeight = window->isFullScreen ? 0 : 16;
	int availableHeight = bounds.size.h - titleBarHeight - statusBarHeight;
	if(availableHeight < 3){
		availableHeight = 3;
	}
	int regionHeight = availableHeight / 3;
	int horizontalPadding = 0;
	int labelWidth = bounds.size.w;

	TextLayer * layers[3] = {
		autoPebbleQuickScreen->textLayerTop,
		autoPebbleQuickScreen->textLayerMiddle,
		autoPebbleQuickScreen->textLayerBottom
	};
	char * labels[3] = {
		autoPebbleQuickScreen->labelTop,
		autoPebbleQuickScreen->labelMiddle,
		autoPebbleQuickScreen->labelBottom
	};

	for(int i = 0; i < 3; i++){
		layer_set_frame(text_layer_get_layer(layers[i]),
			GRect(horizontalPadding, titleBarHeight + (i * regionHeight), labelWidth, regionHeight));
		quickscreen_prepare_label(layers[i], labels[i], window->textFont, bounds);

		GSize labelSize = text_layer_get_content_size(layers[i]);
		int layerHeight = labelSize.h + 4;
		if(layerHeight > regionHeight){
			layerHeight = regionHeight;
		}
		int regionTop = titleBarHeight + (i * regionHeight);
		int y = regionTop + ((regionHeight - layerHeight) / 2);
		layer_set_frame(text_layer_get_layer(layers[i]),
			GRect(horizontalPadding, y, labelWidth, layerHeight));
	}

	Layer *root = window_get_root_layer(window->window);
	layer_mark_dirty(root);
}