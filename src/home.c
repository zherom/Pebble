#include <pebble.h>
#include "home.h"
  
static int lunch_total = 60 * 60;
  
static int forward_uptime = 0;
static int lunch_uptime = 0;
static int backward_uptime = 0;

static int lunch_status = 0;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_forward_icon_white;
static GBitmap *s_res_restaurant_icon;
static GBitmap *s_res_backward_icon_white;
static GFont s_res_font_digit_bold_38;
static ActionBarLayer *actionbarlayer;
static TextLayer *backward_timer;
static TextLayer *lunch_timer;
static TextLayer *forward_timer;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, false);
  #endif
  
  s_res_forward_icon_white = gbitmap_create_with_resource(RESOURCE_ID_forward_icon_WHITE);
  s_res_restaurant_icon = gbitmap_create_with_resource(RESOURCE_ID_restaurant_icon);
  s_res_backward_icon_white = gbitmap_create_with_resource(RESOURCE_ID_backward_icon_WHITE);
  s_res_font_digit_bold_38 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIGIT_BOLD_38));
  // actionbarlayer
  actionbarlayer = action_bar_layer_create();
  action_bar_layer_add_to_window(actionbarlayer, s_window);
  action_bar_layer_set_background_color(actionbarlayer, GColorBlack);
  action_bar_layer_set_icon(actionbarlayer, BUTTON_ID_UP, s_res_forward_icon_white);
  action_bar_layer_set_icon(actionbarlayer, BUTTON_ID_SELECT, s_res_restaurant_icon);
  action_bar_layer_set_icon(actionbarlayer, BUTTON_ID_DOWN, s_res_backward_icon_white);
  layer_add_child(window_get_root_layer(s_window), (Layer *)actionbarlayer);
  
  // backward_timer
  backward_timer = text_layer_create(GRect(0, 110, 110, 40));
  text_layer_set_text(backward_timer, "00:00");
  text_layer_set_text_alignment(backward_timer, GTextAlignmentCenter);
  text_layer_set_font(backward_timer, s_res_font_digit_bold_38);
  layer_add_child(window_get_root_layer(s_window), (Layer *)backward_timer);
  
  // lunch_timer
  lunch_timer = text_layer_create(GRect(0, 60, 110, 40));
  text_layer_set_text(lunch_timer, "60:00");
  text_layer_set_text_alignment(lunch_timer, GTextAlignmentCenter);
  text_layer_set_font(lunch_timer, s_res_font_digit_bold_38);
  layer_add_child(window_get_root_layer(s_window), (Layer *)lunch_timer);
  
  // forward_timer
  forward_timer = text_layer_create(GRect(0, 10, 110, 40));
  text_layer_set_text(forward_timer, "00:00");
  text_layer_set_text_alignment(forward_timer, GTextAlignmentCenter);
  text_layer_set_font(forward_timer, s_res_font_digit_bold_38);
  layer_add_child(window_get_root_layer(s_window), (Layer *)forward_timer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(actionbarlayer);
  text_layer_destroy(backward_timer);
  text_layer_destroy(lunch_timer);
  text_layer_destroy(forward_timer);
  gbitmap_destroy(s_res_forward_icon_white);
  gbitmap_destroy(s_res_restaurant_icon);
  gbitmap_destroy(s_res_backward_icon_white);
  fonts_unload_custom_font(s_res_font_digit_bold_38);
}
// END AUTO-GENERATED UI CODE

/*static void write_to_label(int mytimer, TextLayer *label, char *s_buffer) {
  //Get rest time since launch
  int m_minutes = (mytimer % 3600) / 60;
  int m_seconds = mytimer % 60;
  
  // Update the TextLayer
  snprintf(s_buffer, sizeof(s_buffer), "%02d:%02d", m_minutes, m_seconds);
  text_layer_set_text(label, s_buffer);
}*/

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // Use a long-lived buffer
  //static char forward_s_buffer[32];
  //static char lunch_s_buffer[32];
  //static char backward_s_buffer[32];
  if (lunch_status == 1) {
    //write_to_label(forward_uptime, forward_timer, forward_s_buffer);
    //write_to_label(lunch_total - 2*forward_uptime, lunch_timer, lunch_s_buffer);
    //write_to_label(forward_uptime, backward_timer, backward_s_buffer);
    
    // Use a long-lived buffer
    static char s_uptime_buffer[32];
    static char s_downtime_buffer[32];
  
    // Get time since launch
    int seconds = forward_uptime % 60;
    int minutes = (forward_uptime % 3600) / 60;
    
    //Get rest time since launch
    int time_elapsed = lunch_total - 2*forward_uptime;
    int down_seconds = 0;
    int down_minutes = 0;
    if (time_elapsed >= 0) {
      down_seconds = time_elapsed % 60;
      down_minutes = (time_elapsed % 3600) / 60;
    } else {
      vibes_double_pulse();
    }
    
    // Update the TextLayer
    snprintf(s_uptime_buffer, sizeof(s_uptime_buffer), "%02d:%02d", minutes, seconds);
    snprintf(s_downtime_buffer, sizeof(s_downtime_buffer), "%02d:%02d", down_minutes, down_seconds);
    text_layer_set_text(forward_timer, s_uptime_buffer);
    text_layer_set_text(backward_timer, s_uptime_buffer);
    text_layer_set_text(lunch_timer, s_downtime_buffer);
  
    // Increment s_uptime
    forward_uptime++;
    
  } else if (lunch_status == 2) {
    // Use a long-lived buffer
    static char s_downtime_buffer[32];
    
    //Get rest time since launch
    int time_elapsed = lunch_total - 2 * forward_uptime - lunch_uptime;
    int down_seconds = time_elapsed % 60;
    int down_minutes = (time_elapsed % 3600) / 60;
    if (time_elapsed >= 0){
      down_seconds = time_elapsed % 60;
      down_minutes = (time_elapsed % 3600) / 60;
    } else {
      vibes_double_pulse();
      down_seconds = lunch_uptime % 60;
      down_minutes = (lunch_uptime % 3600) / 60;
    }
    
  
    // Update the TextLayer
    snprintf(s_downtime_buffer, sizeof(s_downtime_buffer), "%02d:%02d", down_minutes, down_seconds);
    text_layer_set_text(lunch_timer, s_downtime_buffer);
  
    // Increment s_uptime
    lunch_uptime++;
  } else if (lunch_status == 3) {
    // Use a long-lived buffer
    static char s_downtime_buffer[32];
    static char s_lunch_buffer[32];
    
    //Get rest time since launch
    int l_seconds = lunch_uptime % 60;
    int l_minutes = (lunch_uptime % 3600) / 60;
    
    int time_elapsed = lunch_total - forward_uptime - lunch_uptime - backward_uptime;
    int down_seconds = 0;
    int down_minutes = 0;
    if (time_elapsed >= 0) {
      down_seconds = time_elapsed % 60;
      down_minutes = (time_elapsed % 3600) / 60;
    } else {
      vibes_double_pulse();
      down_seconds = backward_uptime % 60;
      down_minutes = (backward_uptime % 3600) / 60;
    }
  
    // Update the TextLayer
    snprintf(s_downtime_buffer, sizeof(s_downtime_buffer), "%02d:%02d", down_minutes, down_seconds);
    snprintf(s_lunch_buffer, sizeof(s_lunch_buffer), "%02d:%02d", l_minutes, l_seconds);
    text_layer_set_text(lunch_timer, s_lunch_buffer);
    text_layer_set_text(backward_timer, s_downtime_buffer);
  
    // Increment s_uptime
    backward_uptime++;
  } else if (lunch_status == 4) {
    // Use a long-lived buffer
    static char s_backwardtime_buffer[32];
    
    //Get rest time since launch
    int b_seconds = backward_uptime % 60;
    int b_minutes = (backward_uptime % 3600) / 60;
  
    // Update the TextLayer
    snprintf(s_backwardtime_buffer, sizeof(s_backwardtime_buffer), "%02d:%02d", b_minutes, b_seconds);
    text_layer_set_text(backward_timer, s_backwardtime_buffer);
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (lunch_status == 0) {//setmode for lunch duration
    lunch_status = -1;
  } else if (lunch_status == -1) {//exit set mode
    lunch_status = 0;
  } else if (lunch_status == 1) {
    lunch_status= 2;
  }
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (lunch_status == -1) {
    if (lunch_total >= 90 * 60)
      lunch_total = 60;
    else 
      lunch_total += 60;
    
    // Use a long-lived buffer
    static char s_backwardtime_buffer[32];
     
    // Update the TextLayer
    snprintf(s_backwardtime_buffer, sizeof(s_backwardtime_buffer), "%02d:%02d", lunch_total / 60, 0);
    text_layer_set_text(lunch_timer, s_backwardtime_buffer);
  } else if (lunch_status == 0) {
    lunch_status = 1;
  } else if (lunch_status == 4) {
    lunch_status = 0;
    forward_uptime = 0;
    lunch_uptime = 0;
    backward_uptime = 0;
    
    static char s_backwardtime_buffer[32];
    text_layer_set_text(backward_timer, "00:00");
    text_layer_set_text(forward_timer, "00:00");
    snprintf(s_backwardtime_buffer, sizeof(s_backwardtime_buffer), "%02d:%02d", lunch_total / 60, 0);
    text_layer_set_text(lunch_timer, s_backwardtime_buffer);
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (lunch_status == -1) {
    if (lunch_total <= 60)
      lunch_total = 90 * 60;
    else 
      lunch_total -= 60;
    
    // Use a long-lived buffer
    static char s_backwardtime_buffer[32];
  
    // Update the TextLayer
    snprintf(s_backwardtime_buffer, sizeof(s_backwardtime_buffer), "%02d:%02d", lunch_total / 60, 0);
    text_layer_set_text(lunch_timer, s_backwardtime_buffer);
  } else if (lunch_status == 2) {
    lunch_status = 3;
  } else if (lunch_status == 3) {
    lunch_status = 4;
  }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_home(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
  window_set_click_config_provider(s_window, click_config_provider);
  
  // Subscribe to TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

void hide_home(void) {
  window_stack_remove(s_window, true);
}