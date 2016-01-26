#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;
static Layer *s_canvas;
static GPath *s_white_triangle_path = NULL;
static const GPathInfo TRIANGLE_PATH_INFO = {
  .num_points = 3,
  .points = (GPoint []) {{88, 129}, {92, 97}, {96, 129}}
};
static GPath *s_flame_triangle_path = NULL;
static const GPathInfo FLAME_TRIANGLE_PATH_INFO = {
  .num_points = 3,
  .points = (GPoint []) {{84, 144}, {92, 170}, {100, 144}}
};
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[15];
  PBL_IF_ROUND_ELSE(strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H     %M" : "%I     %M", tick_time), 
                    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H   %M" : "%I   %M", tick_time));

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
static void draw_for_round(Layer *layer, GContext *ctx){
  //draw top gray segment
  graphics_context_set_fill_color(ctx, GColorLightGray); 
  graphics_fill_rect(ctx, GRect(85, 10, 15, 40), 4, GCornersTop);
  
  //draw gridfins
  graphics_context_set_fill_color(ctx, GColorWhite); 
  //graphics_fill_rect(ctx, GRect(89, 25, 7, 4), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(75, 25, 7, 4), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(103, 25, 7, 4), 0, GCornerNone);
    
  //draw middle light segment
  graphics_context_set_fill_color(ctx, GColorWhite); 
  graphics_fill_rect(ctx, GRect(85, 50, 15, 40), 4, GCornerNone);
  
  //draw bottom dark segment
  graphics_context_set_fill_color(ctx, GColorDarkGray); 
  graphics_fill_rect(ctx, GRect(85, 90, 15, 40), 4, GCornersBottom);
  
  //draw flame
  graphics_context_set_fill_color(ctx, GColorOrange); 
  graphics_fill_circle(ctx, GPoint(92, 140), 8);
  s_flame_triangle_path = gpath_create(&FLAME_TRIANGLE_PATH_INFO);
  gpath_draw_filled(ctx, s_flame_triangle_path);
  
  //draw leg outline
  graphics_context_set_fill_color(ctx, GColorWhite); 
  s_white_triangle_path = gpath_create(&TRIANGLE_PATH_INFO);
  gpath_draw_filled(ctx, s_white_triangle_path);
  
  //draw left legs
  graphics_context_set_stroke_width(ctx, 3);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, GPoint(85, 115), GPoint(60, 135));
  graphics_draw_line(ctx, GPoint(85, 128), GPoint(60, 135));
  
  //draw right legs
  graphics_draw_line(ctx, GPoint(100, 115), GPoint(125, 135));
  graphics_draw_line(ctx, GPoint(100, 128), GPoint(125, 135));
  
  //draw front leg
  graphics_draw_line(ctx, GPoint(92, 115), GPoint(92, 135));
}
static void draw_for_square(Layer *layer, GContext *ctx){
  int square_offset = -20;
  //draw top gray segment
  graphics_context_set_fill_color(ctx, GColorLightGray); 
  graphics_fill_rect(ctx, GRect(85 + square_offset, 10, 15, 40), 4, GCornersTop);
  
  //draw gridfins
  graphics_context_set_fill_color(ctx, GColorWhite); 
  graphics_fill_rect(ctx, GRect(75 + square_offset, 25, 7, 4), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(103 + square_offset, 25, 7, 4), 0, GCornerNone);
    
  //draw middle light segment
  graphics_context_set_fill_color(ctx, GColorWhite); 
  graphics_fill_rect(ctx, GRect(85 + square_offset, 50, 15, 40), 4, GCornerNone);
  
  //draw bottom dark segment
  graphics_context_set_fill_color(ctx, GColorDarkGray); 
  graphics_fill_rect(ctx, GRect(85 + square_offset, 90, 15, 40), 4, GCornersBottom);
  
  //draw flame
  graphics_context_set_fill_color(ctx, GColorOrange); 
  graphics_fill_circle(ctx, GPoint(92 + square_offset, 140), 8);
  s_flame_triangle_path = gpath_create(&FLAME_TRIANGLE_PATH_INFO);
  gpath_move_to(s_flame_triangle_path, GPoint(square_offset, 0));
  gpath_draw_filled(ctx, s_flame_triangle_path);
  
  //draw leg outline
  graphics_context_set_fill_color(ctx, GColorWhite); 
  s_white_triangle_path = gpath_create(&TRIANGLE_PATH_INFO);
  gpath_move_to(s_white_triangle_path, GPoint(square_offset, 0));
  gpath_draw_filled(ctx, s_white_triangle_path);
  
  //draw left legs
  graphics_context_set_stroke_width(ctx, 3);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, GPoint(85 + square_offset, 115), GPoint(60 + square_offset, 135));
  graphics_draw_line(ctx, GPoint(85 + square_offset, 128), GPoint(60 + square_offset, 135));
  
  //draw right legs
  graphics_draw_line(ctx, GPoint(100 + square_offset, 115), GPoint(125 + square_offset, 135));
  graphics_draw_line(ctx, GPoint(100 + square_offset, 128), GPoint(125 + square_offset, 135));
  
  //draw front leg
  graphics_draw_line(ctx, GPoint(92 + square_offset, 115), GPoint(92 + square_offset, 135));
}
static void layer_update_proc(Layer *layer, GContext *ctx) {
   PBL_IF_ROUND_ELSE(draw_for_round(layer, ctx), draw_for_square(layer, ctx));
}
static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //create layer for drawing
  s_canvas = layer_create(bounds);
  layer_set_update_proc(s_canvas, layer_update_proc);
  layer_add_child(window_layer, s_canvas);
  
  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, 62, bounds.size.w, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  layer_destroy(s_canvas);

}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_set_background_color(s_main_window, GColorPictonBlue);
  window_stack_push(s_main_window, true);
  
  // Make sure the time is displayed from the start
  update_time();
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}