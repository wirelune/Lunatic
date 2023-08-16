#include <pebble.h>
#include "Lunatic.h"

// Core stuff before I made everything more interactive
static Window *s_main_window;
static TextLayer *s_time_layer, *s_text_layer;
static GBitmap *s_bitmap = NULL;
static BitmapLayer *s_bitmap_layer;
static GBitmapSequence *s_sequence = NULL;


// For changing animations depending on current hour
static BitmapLayer *s_eepy_layer;
static GBitmap *s_eepy_eyes;

// For talking
static TextLayer *s_talk_layer;
char ldict[20][70] = {"And you call yourself lonely?",
                      "Elder sister knows the best, of course.",
                      "Wanna come at my birthday?",
                      "Wanna fly together somewhere?",
                      "It's cold on the moon.",
                      "There was pony-net on the moon, I've been reading a lot.",
                      "My sister bullies me!",
                      "An ocean of tears on dark side of the moon.",
                      "It's so fresh at nights, I like to go out at this time.",
                      "Write me a letter! 'Dear Princess Luna...'",
                      "The world will shudder to know my greatness.",
                      "Royal guards are boring.",
                      "I saw a plushie of myself. Been thinking a lot about it.",
                      "I bought a plushie of myself to sleep with.",
                      "Fan shipping makes me sad.",
                      "I am the oldest youngest sister in Equestria.",
                      "Celestia, why are you so happy?",
                      "Lunar Republic is the future of Equestria.",
                      "Make a wish upon a star!",
                      "Bow to your princess!"};

// Clay stuff goes right here

ClaySettings settings;

// Initialize the default settings
static void prv_default_settings() {
  settings.lsleep = 16;
  settings.lwake = 8;
  settings.uwake = 23;
  settings.noautism = 0;
}

// Read settings from persistent storage
static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Save the settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  // Update the display based on new settings
  prv_update_display();
}

// Update the display elements
static void prv_update_display() {
  // boo!
}

// Handle the response from AppMessage
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Background Color

  Tuple *t_test = dict_find(iter, MESSAGE_KEY_lsleep);
  if (t_test) {

    settings.lsleep = t_test->value->int32;
  }
  Tuple *t_test1 = dict_find(iter, MESSAGE_KEY_lwake);
  if (t_test1) {

    settings.lwake = t_test1->value->int32;
  }
  Tuple *t_test2 = dict_find(iter, MESSAGE_KEY_uwake);
  if (t_test2) {

    settings.uwake = t_test2->value->int32;
  }
  Tuple *t_test3 = dict_find(iter, MESSAGE_KEY_noautism);
  if (t_test3) {

    settings.noautism = t_test3->value->uint8;
  }

  // Save the new settings to persistent storage
  prv_save_settings();
}

// Clay stuff ends here

static void load_sequence();



static void update_time() {

  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  // Another buffer for date
  static char s_buffer2[20];

  // Format the time value
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  strftime(s_buffer2, sizeof(s_buffer2), "%A, %e.%m", tick_time);
  // Display this time from buffer on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
  layer_set_hidden(text_layer_get_layer(s_time_layer), false);
  layer_set_hidden(text_layer_get_layer(s_talk_layer), true);
  // Display date
  text_layer_set_text(s_text_layer, s_buffer2);



  // Getting current hour
  int current_hour = tick_time->tm_hour;
if(settings.noautism == 0)
  {
    // Check time and do things
    if (current_hour >= settings.lsleep) // Princess Luna wakes up!
    {
      layer_set_hidden(bitmap_layer_get_layer(s_eepy_layer), true);
    } else if (current_hour < settings.lwake) // She wakes up here too!
    {
      layer_set_hidden(bitmap_layer_get_layer(s_eepy_layer), true);
    } else if (current_hour >= settings.lwake) // She goes to sleep right here
      {
        layer_set_hidden(bitmap_layer_get_layer(s_eepy_layer), false);
      }

        if (current_hour == settings.uwake) // (You)'re going to sleep
        {
          text_layer_set_text(s_text_layer, "Sweet dreams.");
        } else
        text_layer_set_text(s_text_layer, s_buffer2);

    } else if(settings.noautism == 1)
      {
        layer_set_hidden(bitmap_layer_get_layer(s_eepy_layer), true);

      }
}



static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void timer_handler(void *context)
{
  uint32_t next_delay;

  // Advance to next APNG frame and get delay to this frame
  if(gbitmap_sequence_update_bitmap_next_frame(s_sequence, s_bitmap, &next_delay)) {
    // Set the new frame into bitmaplayer
    bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
    layer_mark_dirty(bitmap_layer_get_layer(s_bitmap_layer));

    // Timer for that frame's delay
    app_timer_register(next_delay, timer_handler, NULL);

  } else load_sequence();

}

static void accel_tap_handler(AccelAxisType axis, int32_t direction)
{

  // Display something interesting here
  if(settings.noautism == 0)
    {
      layer_set_hidden(text_layer_get_layer(s_talk_layer), false);
      text_layer_set_text(s_talk_layer, ldict[rand()%20]);
      layer_set_hidden(text_layer_get_layer(s_time_layer), true);
    }

}

static void load_sequence() {

  if(s_sequence) {
    gbitmap_sequence_destroy(s_sequence);
    s_sequence = NULL;
  }
  if(s_bitmap) {
    gbitmap_destroy(s_bitmap);
    s_bitmap = NULL;
  }
  // Create sequence
s_sequence = gbitmap_sequence_create_with_resource(RESOURCE_ID_AWAKE);
GSize anim_frame_size = gbitmap_sequence_get_bitmap_size(s_sequence);

  // Create gbitmap
  s_bitmap = gbitmap_create_blank(anim_frame_size, GBitmapFormat8Bit);

  app_timer_register(1, timer_handler, NULL);

}

// Window Load event
static void prv_window_load(Window *window) {

  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create BitmapLayer to display the GBitmap
  s_bitmap_layer = bitmap_layer_create(bounds);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_bitmap_layer));

  load_sequence();

  // Create the TextLayers for time and date with specific bounds
  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(45, 18), (bounds.size.w), 62));
  s_talk_layer = text_layer_create(GRect(0, 17, (bounds.size.w), 123));
  s_text_layer = text_layer_create(GRect(0, -5, (bounds.size.w), 30));

  // Settings for the time layer
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Settings for talk layer
  text_layer_set_background_color(s_talk_layer, GColorClear);
  text_layer_set_text_color(s_talk_layer, GColorWhite);
  text_layer_set_font(s_talk_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_talk_layer, GTextAlignmentCenter);

  // Same for date layer
  text_layer_set_background_color(s_text_layer, GColorClear);
  text_layer_set_text_color(s_text_layer, GColorWhite);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);

  // Add everything as a child layers to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_talk_layer));

  // Things for eepy
  s_eepy_eyes = gbitmap_create_with_resource(RESOURCE_ID_EEPY);
  s_eepy_layer = bitmap_layer_create(GRect(0, 70, 144, 98));
  bitmap_layer_set_bitmap(s_eepy_layer, s_eepy_eyes);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_eepy_layer));


  prv_update_display();
}

// Window Unload event
static void prv_window_unload(Window *window) {

    // Destroy TextLayers
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_text_layer);
    text_layer_destroy(s_talk_layer);

    // Destroy GBitmap
    gbitmap_destroy(s_bitmap);
    // Destroy BitmapLayer
    bitmap_layer_destroy(s_bitmap_layer);
    // Destroy sequence
    gbitmap_sequence_destroy(s_sequence);

    // Destroy eepy thing
    bitmap_layer_destroy(s_eepy_layer);
    gbitmap_destroy(s_eepy_eyes);

}

static void prv_init(void) {
  prv_load_settings();

  // Listen for AppMessages
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);

  // Create main Window element and assign to pointer
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Make sure the time is displayed from the start
  update_time();

  // Subscribe to tap events
  accel_tap_service_subscribe(accel_tap_handler);
}

static void prv_deinit(void) {
  window_destroy(s_main_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
