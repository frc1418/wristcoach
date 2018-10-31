#include <pebble.h>
#include <stdio.h>
#include <stdbool.h>
#define AUTONOMOUS_LENGTH 15
#define     TELEOP_LENGTH 135
#define      NEAR_ENDGAME 40
#define           ENDGAME 30
static Window *s_main_window;
static TextLayer *s_instructions;
static TextLayer *s_timer;
static TextLayer *s_mode;
static time_t s_start_time;
static bool s_running;

// TODO: be more consistent about declaring functions here
static void start_timer();
static void stop_timer();

static void main_window_load(Window *window) {
    // Get information about the Window
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    GFont custom_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LECO_55));

    // Set up timer box
    s_timer = text_layer_create(GRect(0, 20, bounds.size.w, 70));
    text_layer_set_background_color(s_timer, GColorClear);
    text_layer_set_text_color(s_timer, GColorWhite);
    text_layer_set_text(s_timer, "");
    text_layer_set_font(s_timer, custom_font);
    text_layer_set_text_alignment(s_timer, GTextAlignmentCenter);
    // Insert into window
    layer_add_child(window_layer, text_layer_get_layer(s_timer));

    // Set up message box
    s_mode = text_layer_create(GRect(0, 90, bounds.size.w, 40));
    text_layer_set_background_color(s_mode, GColorWhite);
    text_layer_set_text_color(s_mode, GColorBlack);
    text_layer_set_text(s_mode, "");
    text_layer_set_font(s_mode, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(s_mode, GTextAlignmentCenter);
    // Insert into window
    layer_add_child(window_layer, text_layer_get_layer(s_mode));

    // Set up start instructions
    s_instructions = text_layer_create(GRect(75, 65, bounds.size.w - 75, 40));
    text_layer_set_background_color(s_instructions, GColorBlack);
    text_layer_set_text_color(s_instructions, GColorWhite);
    text_layer_set_text(s_instructions, "START");
    text_layer_set_font(s_instructions, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(s_instructions, GTextAlignmentCenter);
    // Insert into window
    layer_add_child(window_layer, text_layer_get_layer(s_instructions));
}

static void main_window_unload(Window *window) {
    text_layer_destroy(s_timer);
    text_layer_destroy(s_mode);
}

static void update_time() {
    if (s_start_time) {
        // Get a tm structure
        time_t curr_time = time(NULL);
        int remaining = (AUTONOMOUS_LENGTH + TELEOP_LENGTH) - (curr_time - s_start_time);
        // TODO: should we use this and not normal time()?
        //struct tm *tick_time = localtime(&temp);
        char *str = calloc(sizeof(char), 3+1);
        snprintf(str, 3+1, "%d", remaining);

        if (remaining > TELEOP_LENGTH) {
            text_layer_set_text(s_mode, "AUTON");
            window_set_background_color(s_main_window, GColorBlue);
            text_layer_set_text_color(s_mode, GColorBlue);
        } else if (remaining > ENDGAME) {
            text_layer_set_text(s_mode, "TELEOP");
            window_set_background_color(s_main_window, GColorGreen);
            text_layer_set_text_color(s_mode, GColorGreen);
        } else {
            text_layer_set_text(s_mode, "ENDGAME");
            window_set_background_color(s_main_window, GColorRed);
            text_layer_set_text_color(s_mode, GColorRed);
        }

        if (remaining == NEAR_ENDGAME) vibes_short_pulse();
        if (remaining == ENDGAME) vibes_double_pulse();
        if (remaining == 0) stop_timer();

        // Display this time on the TextLayer
        text_layer_set_text(s_timer, str);
    }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static void start_timer() {
    // Start timer (temporary)
    s_start_time = time(NULL);
    // Register with TickTimerService
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    s_running = true;
    text_layer_destroy(s_instructions);
}
static void stop_timer() {
    s_running = false;
    // Unsubscribe from tick event; saves battery
    tick_timer_service_unsubscribe();
    text_layer_set_text(s_timer, "||");
}

static void toggle_timer() {
    if (s_running) stop_timer(); else start_timer();
}

static void select_click_handler() {
    toggle_timer();
}

static void click_config_provider(void *context) {
    ButtonId id = BUTTON_ID_SELECT;
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

    // Set up button click handler
    window_set_click_config_provider(s_main_window, click_config_provider);

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
