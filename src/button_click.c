/*
#include "pebble.h"


static Window *window;

static TextLayer *text_layer;

//The maximum allowed duration is 10000ms.
//VibePattern custom_pattern = {
//  .durations = (uint32_t [50]),
//  .num_segments = 0
//};

//static const uint32_t const segments[] = { 200, 100, 400 };
uint32_t segments[50];// = { 200, 100, 400 };
uint32_t seglen = 10;
int vibon=50, viboff=50;
int cursor=1, togg=1;


void updatetext() {
   //GRect textframe = GRect(0, 0, 143, 20);  // Text Box Position and Size
   // graphics_context_set_fill_color(ctx, 0);   graphics_fill_rect(ctx, textframe, 0, GCornerNone);  //Black Solid Rectangle
   // graphics_context_set_stroke_color(ctx, 1); graphics_draw_rect(ctx, textframe);                //White Rectangle Border  
    static char text[40];  //Buffer to hold text
    if(cursor==0) snprintf(text, sizeof(text), "->vib on: %d\n  vib off: %d\n  vib", vibon, viboff);  // What text to draw  
    if(cursor==1) snprintf(text, sizeof(text), "  vib on: %d\n->vib off: %d\n  vib", vibon, viboff);  // What text to draw  
    if(cursor==2) snprintf(text, sizeof(text), "  vib on: %d\n  vib off: %d\n->vib", vibon, viboff);  // What text to draw  
   // graphics_context_set_text_color(ctx, 1);  // White Text
   // graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_14), textframe, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);  //Write Text
  
  text_layer_set_text(text_layer, text);
 

}

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(togg>0) {
      cursor--; if(cursor==-1) cursor=2;
  } else {
    if(cursor==0) {vibon += 1; if(vibon>90) vibon=90;}
    if(cursor==1) {viboff += 1; if(viboff>90) viboff=90;}  
  }
  updatetext();
  //vibes_short_pulse();
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  //vibes_long_pulse();
  if(togg>0)
    cursor=(cursor + 1) % 3;
  else
  {
    if(cursor==0) {vibon -= 5; if(vibon<0) vibon=0;}
    if(cursor==1) {viboff -= 5; if(viboff<0) viboff=0;}  
  }
  updatetext();

  //vibes_enqueue_custom_pattern(custom_pattern);
}

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  //vibes_double_pulse();
  if(cursor<2) togg *= -1;  
  if(cursor==2) {
  int i;
  for(i=0; i<50; i+=2) {
    segments[i] = vibon;
    segments[i+1] = viboff;
  }
  seglen = 48;
  VibePattern pat = {
    .durations = segments,
    .num_segments = ARRAY_LENGTH(segments),
  };
  vibes_enqueue_custom_pattern(pat);
  }
}

void click_config_provider(void *context) {
  const uint16_t repeat_interval_ms = 1000;
  window_single_repeating_click_subscribe(BUTTON_ID_SELECT, repeat_interval_ms, select_single_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_UP, repeat_interval_ms, up_single_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, repeat_interval_ms, down_single_click_handler);
}

int main(void) {
  window = window_create();
  window_stack_push(window, true); // Animated

  window_set_click_config_provider(window, click_config_provider);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  text_layer = text_layer_create(bounds);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  updatetext();
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));

  

  //{10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30, 10, 30}
  
  app_event_loop();

  text_layer_destroy(text_layer);
  window_destroy(window);
}
*/