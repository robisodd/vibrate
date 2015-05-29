#include "pebble.h"
#define mul 1

static Window *window;

static TextLayer *text_layer;

//The maximum allowed duration is 10000ms.
//VibePattern custom_pattern = {
//  .durations = (uint32_t [50]),
//  .num_segments = 0
//};

//static const uint32_t const segments[] = { 200, 100, 400 };
//uint32_t segments[110];
//uint32_t seglen = ARRAY_LENGTH(segments);

int vibon=5;//, viboff=10;
/*
void dovib() {
  int i;
  if (vibon>11) {
   if (vibon == 12) vibes_short_pulse(); // 250
   if (vibon == 13) vibes_long_pulse();  // 500
   if (vibon == 14){
     for(i=0; i<30; i+=2) {
       segments[i] = 3; // On
       segments[i+1] = 7;//10-vibon // off
     }
     segments[30]=0;
     segments[31]=250;
     segments[32]=250;
     segments[33]=250;
     for(i=34; i<54; i+=2) {
       segments[i] = 3; // On
       segments[i+1] = 7;//10-vibon // off
     }
     segments[54]=0;
     segments[55]=250;
     segments[56]=250;
     segments[57]=250;
     for(i=58; i<78; i+=2) {
       segments[i] = 3; // On
       segments[i+1] = 7;//10-vibon // off
     }
     segments[78]=0;
     segments[79]=250;
     segments[80]=250;
     segments[81]=250;
     for(i=82; i<102; i+=2) {
       segments[i] = 3; // On
       segments[i+1] = 7;//10-vibon // off
     }
     segments[102]=0;
     segments[103]=250;
     segments[104]=250;
     segments[105]=250;
     
     seglen=106;
     VibePattern pat = {
      .durations = segments,
      .num_segments = seglen, //ARRAY_LENGTH(segments)
     };
     vibes_enqueue_custom_pattern(pat);
   }
  } else {  
    if (vibon==11) {
      segments[0] = 250; // on
      seglen=1;
    } else {
      for(i=0; i<50; i+=2) {
        segments[i] = vibon * mul; // On
        segments[i+1] = 0;//(10-vibon) * mul;  // off
      }
      seglen = 30;
    }  
    VibePattern pat = {
      .durations = segments,
      .num_segments = seglen, //ARRAY_LENGTH(segments)
    };
    vibes_enqueue_custom_pattern(pat);
  }
}
*/
#define MAX_PWM_DURATION 1000 // Longest duration (in ms). Bigger makes larger array
// Pulse-Width Modulation Vibration
// strength: pwm ranges from 0 - 10, <0 is fully off, >10 is fully on
// duration: duration in milliseconds (must be > 0)
void vibes_pwm(int32_t strength, uint32_t duration) {
  uint32_t pwm_segments[MAX_PWM_DURATION/7], pwm_seglen;
  if(strength < 0) {
    vibes_cancel();
  } else if(strength > 10) {
    pwm_segments[0] = duration;  // on for [duration] (in ms)
    VibePattern pwm_pat = {.durations = pwm_segments, .num_segments = 1};
    vibes_enqueue_custom_pattern(pwm_pat);
  } else {
    if(duration > MAX_PWM_DURATION) duration = MAX_PWM_DURATION;
    duration /= 7;  // Should be 5. Trial&Error found 7
    for(pwm_seglen = 0; pwm_seglen < duration; pwm_seglen++) {
      pwm_segments[pwm_seglen] = strength;      // on duration (in ms)
      pwm_segments[++pwm_seglen] = 10 - strength; // off duration (in ms)
     }
    VibePattern pwm_pat = {.durations = pwm_segments, .num_segments = pwm_seglen};
    vibes_enqueue_custom_pattern(pwm_pat);
  }
}

void updatetext() {
   //GRect textframe = GRect(0, 0, 143, 20);  // Text Box Position and Size
   // graphics_context_set_fill_color(ctx, 0);   graphics_fill_rect(ctx, textframe, 0, GCornerNone);  //Black Solid Rectangle
   // graphics_context_set_stroke_color(ctx, 1); graphics_draw_rect(ctx, textframe);                //White Rectangle Border  
    static char text[40];  //Buffer to hold text
    //if(cursor==0) snprintf(text, sizeof(text), "->vib on: %d\n  vib off: %d\n  vib", vibon, viboff);  // What text to draw  
    //if(cursor==1) snprintf(text, sizeof(text), "  vib on: %d\n->vib off: %d\n  vib", vibon, viboff);  // What text to draw  
    //if(cursor==2) snprintf(text, sizeof(text), "  vib on: %d\n  vib off: %d\n->vib", vibon, viboff);  // What text to draw  
   snprintf(text, sizeof(text), "  %d0%%", vibon);  // What text to draw  
   // graphics_context_set_text_color(ctx, 1);  // White Text
   // graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_14), textframe, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);  //Write Text
  
  text_layer_set_text(text_layer, text);
 

}

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  vibon += 1; if(vibon>14) vibon=14;
  updatetext();
  //dovib();
  vibes_pwm(vibon, 500);
  //vibes_short_pulse();
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  //vibes_long_pulse();
  vibon -= 1; if(vibon<-10) vibon=-10;
  updatetext();
  //dovib();
  vibes_pwm(vibon, 500);
  //vibes_enqueue_custom_pattern(custom_pattern);
}

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  //vibes_double_pulse();
  //dovib();
  vibes_pwm(vibon, 500);
}

void click_config_provider(void *context) {
  const uint16_t repeat_interval_ms = 1000;
  window_single_repeating_click_subscribe(BUTTON_ID_SELECT, repeat_interval_ms, select_single_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_UP, repeat_interval_ms, up_single_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, repeat_interval_ms, down_single_click_handler);
}

int main(void) {
  window = window_create();
  window_stack_push(window, true); // true = Animated

  window_set_click_config_provider(window, click_config_provider);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  text_layer = text_layer_create(bounds);
  text_layer_set_text(text_layer, "Press buttons for different vibes. Hold for repeating.");
  
  static char text[40];  //Buffer to hold text
  snprintf(text, sizeof(text), "  %d0%%", vibon);  // What text to draw  
  text_layer_set_text(text_layer, text);
  
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  app_event_loop();

  text_layer_destroy(text_layer);
  window_destroy(window);
}
