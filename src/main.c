 /* 
    Binarian Watchface for Pebble
    Copyright (c) 2015 Santiago Santoro <sjsantoro@gmail.com>
 
    Binarian Watchface for Pebble is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Binarian Watchface for Pebble is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include <pebble.h>

static Window* s_main_window;
static Layer* time_layer;
static TextLayer* date_layer;

const char* weekday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char* month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

static int cols[4][4] = {
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};


void makeTimeArray(struct tm* timeinfo)
{
	char hr[5];

	// Print time to string
	snprintf(hr, 5, "%02d%02d", timeinfo->tm_hour, timeinfo->tm_min);

	int i = 0;
	int j = 0;

	// Parse columns
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if((hr[j] >> (3 - i)) & 0x01)
				cols[i][j] = 1;
			else
				cols[i][j] = 0;
		}
	}
}

static void display_date(struct tm* timeinfo)
{
  static char date_string[16];
  
  // Format date string
  snprintf(date_string, 16, "%s %s %d %d", weekday[timeinfo->tm_wday], month[timeinfo->tm_mon], timeinfo->tm_mday, (timeinfo->tm_year + 1900));
  
  text_layer_set_text(date_layer, date_string);
}

static void draw_time_layer(Layer* layer, GContext* ctx)
{
  // Set color
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorWhite);
  
  // Draw first column of hours
  if(cols[2][0] == 1)
    graphics_fill_circle(ctx, GPoint(12, 58 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(12, 58 + 12), 12);
  
  if(cols[3][0] == 1)
    graphics_fill_circle(ctx, GPoint(12, 84 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(12, 84 + 12), 12);
  
  // Draw second column of hours
  if(cols[0][1] == 1)
    graphics_fill_circle(ctx, GPoint(40, 3 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(40, 3 + 12), 12);
  
  if(cols[1][1] == 1)
    graphics_fill_circle(ctx, GPoint(40, 30 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(40, 30 + 12), 12);
  
  if(cols[2][1] == 1)
    graphics_fill_circle(ctx, GPoint(40, 58 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(40, 58 + 12), 12);
  
  if(cols[3][1] == 1)
    graphics_fill_circle(ctx, GPoint(40, 84 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(40, 84 + 12), 12);
  
  // Draw first column of minutes
  if(cols[1][2] == 1)
    graphics_fill_circle(ctx, GPoint(80, 30 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(80, 30 + 12), 12);
  
  if(cols[2][2] == 1)
    graphics_fill_circle(ctx, GPoint(80, 58 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(80, 58 + 12), 12);
  
  if(cols[3][2] == 1)
    graphics_fill_circle(ctx, GPoint(80, 84 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(80, 84 + 12), 12);
  
    // Draw second column of minutes
  if(cols[0][3] == 1)
    graphics_fill_circle(ctx, GPoint(108, 3 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(108, 3 + 12), 12);
  
  if(cols[1][3] == 1)
    graphics_fill_circle(ctx, GPoint(108, 30 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(108, 30 + 12), 12);
  
  if(cols[2][3] == 1)
    graphics_fill_circle(ctx, GPoint(108, 58 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(108, 58 + 12), 12);
  
  if(cols[3][3] == 1)
    graphics_fill_circle(ctx, GPoint(108, 84 + 12), 12);
  else
    graphics_draw_circle(ctx, GPoint(108, 84 + 12), 12);
}

static void update_time()
{
  time_t temp = time(NULL);
  struct tm* tick_time = localtime(&temp);
  
  makeTimeArray(tick_time);
  
  // Update date
  display_date(tick_time);
  
  // Mark time layer as dirty
  layer_mark_dirty(time_layer);
}

static void tick_handler(struct tm* tick_time, TimeUnits units_changed)
{
  update_time();
}

static void main_window_load(Window* window)
{
  // Get root layer for the window.
  Layer* window_layer = window_get_root_layer(window);
  
  // Set background color to black
  window_set_background_color(window, GColorBlack);
  
  // Create a layer for the time and add it as a child layer
  time_layer = layer_create(GRect(12, 3, 132, 109));
  layer_set_update_proc(time_layer, draw_time_layer);
  layer_add_child(window_layer, time_layer);
  
  // Create date layer and add it as a child layer
  date_layer = text_layer_create(GRect(0, 131, 144, 166));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_background_color(date_layer, GColorBlack);
  text_layer_set_text_color(date_layer, GColorWhite);
  layer_add_child(window_layer, text_layer_get_layer(date_layer));
}

static void main_window_unload(Window* window)
{
  layer_destroy(time_layer);
  text_layer_destroy(date_layer);
}

static void init()
{
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_main_window, true);
  
  update_time();
  
  // Register with timer service
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit()
{
  // Unegister with timer service
  tick_timer_service_unsubscribe();
  
  window_destroy(s_main_window);
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}
  