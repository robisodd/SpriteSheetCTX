#include "pebble.h"

Window *main_window;
Layer *root_layer;
GBitmap *font8_bmp; uint8_t *font8;

void draw_font8_ctx(GContext *ctx, int16_t start_x, int16_t start_y, uint8_t chr) {
    uint8_t *row = font8 + (chr&3) + ((chr&252)*8);  // point to top row (8 pixels) of tile
    for(uint32_t y=0; y<8; y++, row+=4) {  // row+=4 increments to next row
      for(uint32_t x=0; x<8; x++) {
#ifdef PBL_BW
        if(*row & (1<<x))  // *row = 8 bits of row.  This masks the desired pixel
#else
        if(*row & (128>>x))  // *row = 8 bits of row.  This masks the desired pixel
#endif
          graphics_context_set_stroke_color(ctx, GColorWhite);
        else
          graphics_context_set_stroke_color(ctx, GColorBlack);
        graphics_draw_pixel(ctx, GPoint(start_x+x,start_y+y));
      }
    }
}

void root_layer_update(Layer *me, GContext *ctx) {
  draw_font8_ctx(ctx, 5, 5, 'A');
  draw_font8_ctx(ctx, 10, 10, 'B');
  for(uint8_t i=0; i<10; i++)
    draw_font8_ctx(ctx, 10 + i*8, 100, 48+i);
}

// ------------------------------------------------------------------------ //
//  Main Functions
// ------------------------------------------------------------------------ //
static void main_window_load(Window *window) {
  root_layer = window_get_root_layer(window);
  layer_set_update_proc(root_layer, root_layer_update);
}

static void init(void) {
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
  });
  window_set_fullscreen(main_window, true);

  font8_bmp = gbitmap_create_with_resource(RESOURCE_ID_FONT8);
  font8 = gbitmap_get_data(font8_bmp);
 
  window_stack_push(main_window, true); // Display window
}
  
static void deinit(void) {
  gbitmap_destroy(font8_bmp);
  window_destroy(main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}