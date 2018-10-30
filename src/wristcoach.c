#include <pebble.h>
#include <stdio.h>
#define AUTONOMOUS_LENGTH 15
#define     TELEOP_LENGTH 135
#define           ENDGAME 30
static Window *s_main_window;
static TextLayer *s_header;
static TextLayer *s_timer;
static TextLayer *s_message;
static time_t s_start_time;

static void main_window_load(Window *window) {
    // Get information about the Window
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    // Set up header box
    s_header = text_layer_create(GRect(0, 0, bounds.size.w, 30));
    text_layer_set_background_color(s_header, GColorClear);
    text_layer_set_text_color(s_header, GColorBlack);
    text_layer_set_text(s_header, "WristCoach by 1418");
    text_layer_set_font(s_header, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
    text_layer_set_text_alignment(s_header, GTextAlignmentCenter);
    // Insert into window
    layer_add_child(window_layer, text_layer_get_layer(s_header));

    // Set up timer box
    s_timer = text_layer_create(GRect(0, 30, bounds.size.w, 80));
    text_layer_set_background_color(s_timer, GColorClear);
    text_layer_set_text_color(s_timer, GColorBlack);
    text_layer_set_text(s_timer, "");
    text_layer_set_font(s_timer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
    text_layer_set_text_alignment(s_timer, GTextAlignmentCenter);
    // Insert into window
    layer_add_child(window_layer, text_layer_get_layer(s_timer));

    // Set up message box
    s_message = text_layer_create(GRect(0, 110, bounds.size.w, 60));
    text_layer_set_background_color(s_message, GColorClear);
    text_layer_set_text_color(s_message, GColorBlack);
    text_layer_set_text(s_message, "Start >");
    text_layer_set_font(s_message, fonts_get_system_font(FONT_KEY_GOTHIC_24));
    text_layer_set_text_alignment(s_message, GTextAlignmentCenter);
    // Insert into window
    layer_add_child(window_layer, text_layer_get_layer(s_message));
}

static void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_header);
}

static void update_time() {
    // Get a tm structure
    time_t curr_time = time(NULL);
    int remaining = (AUTONOMOUS_LENGTH + TELEOP_LENGTH) - (curr_time - s_start_time);
    // TODO: should we use this and not normal time()?
    //struct tm *tick_time = localtime(&temp);
    char *str = calloc(sizeof(char), 4+1);
    snprintf(str, 4+1, "%ds", remaining);

    if (remaining > TELEOP_LENGTH) {
        text_layer_set_text(s_message, "Autonomous");
        text_layer_set_background_color(s_timer, GColorBlue);
        text_layer_set_text_color(s_timer, GColorWhite);
    } else if (remaining > ENDGAME) {
        text_layer_set_text(s_message, "Teleoperated");
        text_layer_set_background_color(s_timer, GColorGreen);
    } else {
        text_layer_set_text(s_message, "Endgame");
        text_layer_set_background_color(s_timer, GColorRed);
    }

    // Display this time on the TextLayer
    text_layer_set_text(s_timer, str);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static void toggle_timer() {
    if (s_start_time) { // If set, the timer is running. We avoid keeping a time_running boolean this way
        // Remove start_time variable
        s_start_time = NULL;
        // Unsubscribe from tick event; saves battery
        tick_timer_service_unsubscribe(SECOND_UNIT);
    } else { // Timer hasn't been started so do so
        // Register with TickTimerService
        tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
        // Start timer (temporary)
        s_start_time = time(NULL);
    }
}

static void select_click_handler() {
    // TODO: eta if there's nothing else to do
    toggle_timer();
}

static void click_config_provider(void *context) {
    ButtonId id = BUTTON_ID_SELECT;  // The Select button
    window_single_click_subscribe(id, select_click_handler);
}

static void init() {
    // Create main Window element and assign to pointer
    s_main_window = window_create();

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });

    // Show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);

}

static void deinit() {

}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
