/*

The MIT License (MIT)

Copyright (c) 2016 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/
#include <pebble.h>
static Window *my_window;
static TextLayer *text_layer;
static Layer *myCanvas;
static GBitmap *current_background;
static unsigned char buttons[3];

static void sel_click_handler(ClickRecognizerRef recognizer, void *context) {buttons[0] = 1;}
static void sel_release_handler(ClickRecognizerRef recognizer, void *context) {buttons[0] = 0;}
static void sel_click_handler_up(ClickRecognizerRef recognizer, void *context) {buttons[1] = 1;}
static void sel_release_handler_up(ClickRecognizerRef recognizer, void *context) {buttons[1] = 0;}
static void sel_click_handler_down(ClickRecognizerRef recognizer, void *context) {buttons[2] = 1;}
static void sel_release_handler_down(ClickRecognizerRef recognizer, void *context) {buttons[2] = 0;}


static unsigned char anybutton_pressed()
{
	if (buttons[0] == 1 || buttons[1] == 1 || buttons[2] == 1)
		return 1;
		
	return 0;
}

static void Reset_buttons()
{
	buttons[0] = 0;
	buttons[1] = 0;
	buttons[2] = 0;
}

static void timer_handler(void *context)
{
   layer_mark_dirty(myCanvas);
   app_timer_register(34, timer_handler, NULL);
}

static void config_provider(Window *window)
{
   window_raw_click_subscribe(BUTTON_ID_SELECT, sel_click_handler, sel_release_handler, NULL);
   window_raw_click_subscribe(BUTTON_ID_UP, sel_click_handler_up, sel_release_handler_up, NULL);
   window_raw_click_subscribe(BUTTON_ID_DOWN, sel_click_handler_down, sel_release_handler_down, NULL);
}

static void updateGame(Layer *layer, GContext *ctx)
{
	graphics_context_set_compositing_mode(ctx, GCompOpAssign);
	graphics_draw_bitmap_in_rect(ctx, current_background, GRect(0, 0, 144, 168));
	graphics_context_set_text_color(ctx, GColorWhite);
	graphics_draw_text(ctx, "Just pretend you're just friends, okay ?", fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(0, 64, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
			
}

static void init() 
{
   my_window = window_create();
   
   window_set_click_config_provider(my_window, (ClickConfigProvider)config_provider);

   myCanvas = layer_create(GRect(0, 0, 144, 168));

   window_stack_push(my_window, true);
   
   Layer* motherLayer = window_get_root_layer(my_window);
   layer_add_child(motherLayer, myCanvas);

   layer_set_update_proc(myCanvas, updateGame);
   app_timer_register(34, timer_handler, NULL);
}

static void deinit() 
{
   text_layer_destroy(text_layer);
   window_destroy(my_window);
}

int main(void) 
{
	init();
	
	gbitmap_destroy(current_background);
	current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TEST);
	
	app_event_loop();
	deinit();
}
