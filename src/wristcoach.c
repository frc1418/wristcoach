#include <pebble.h>
#include <stdbool.h>
#include "main.h"
#define   AUTONOMOUS_LENGTH 15
#define TELEOPERATED_LENGTH 135
#define      ENDGAME_LENGTH 30

static Window *s_window;
static TextLayer *s_instructions,
                 *s_timer,
                 *s_mode;
static time_t s_start_time;
static bool s_running;

ClaySettings settings;

static void default_settings() {
    settings.EarlyWarningTime = 40;
    settings.EndgameWarningTime = 30;
}

static void load_settings() {
    default_settings();
    persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}
static void save_settings() {
    persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
    Tuple *early_warning_time_t = dict_find(iter, MESSAGE_KEY_EarlyWarningTime);
    if (early_warning_time_t) settings.EarlyWarningTime = early_warning_time_t->value->int32;

    Tuple *endgame_warning_time_t = dict_find(iter, MESSAGE_KEY_EndgameWarningTime);
    if (endgame_warning_time_t) settings.EndgameWarningTime = endgame_warning_time_t->value->int32;

    save_settings();
}

static void window_load(Window *window) {
    // Get information about the Window
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    GFont timer_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LECO_55));

    // Countdown
    s_timer = text_layer_create(GRect(0, 20, bounds.size.w, 70));
    text_layer_set_background_color(s_timer, GColorClear);
    text_layer_set_text_color(s_timer, GColorWhite);
    text_layer_set_font(s_timer, timer_font);
    text_layer_set_text_alignment(s_timer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(s_timer));

    // Mode readout
    s_mode = text_layer_create(GRect(0, 90, bounds.size.w, 40));
    text_layer_set_background_color(s_mode, GColorWhite);
    text_layer_set_font(s_mode, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(s_mode, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(s_mode));

    // Start ribbon
    s_instructions = text_layer_create(GRect(75, 65, bounds.size.w - 75, 40));
    text_layer_set_background_color(s_instructions, GColorBlack);
    text_layer_set_text_color(s_instructions, GColorWhite);
    text_layer_set_text(s_instructions, "START");
    text_layer_set_font(s_instructions, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(s_instructions, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(s_instructions));
}

static void window_unload(Window *window) {
    if (s_timer) text_layer_destroy(s_timer);
    if (s_mode) text_layer_destroy(s_mode);
    if (s_instructions) text_layer_destroy(s_instructions);
}

static void update_time() {
    if (s_start_time) {
        // Get a tm structure
        time_t curr_time = time(NULL);
        int remaining = (AUTONOMOUS_LENGTH + TELEOPERATED_LENGTH) - (curr_time - s_start_time);
        // TODO: should we use this and not normal time()?
        //struct tm *tick_time = localtime(&temp);
        if (remaining > TELEOPERATED_LENGTH) {
            text_layer_set_text(s_mode, "AUTO");
            window_set_background_color(s_window, GColorBlue);
            text_layer_set_text_color(s_mode, GColorBlue);
        } else if (remaining > ENDGAME_LENGTH) {
            text_layer_set_text(s_mode, "TELEOP");
            window_set_background_color(s_window, GColorGreen);
            text_layer_set_text_color(s_mode, GColorGreen);
        } else {
            text_layer_set_text(s_mode, "ENDGAME");
            window_set_background_color(s_window, GColorRed);
            text_layer_set_text_color(s_mode, GColorRed);
        }

        if (remaining == settings.EarlyWarningTime) vibes_short_pulse();
        if (remaining == settings.EndgameWarningTime) vibes_double_pulse();
        if (remaining == 0) stop_timer();

        // Display remaining time
        char *str = calloc(sizeof(char), 3+1);
        snprintf(str, 3+1, "%d", remaining);
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
    if (s_instructions)
        text_layer_destroy(s_instructions);
    update_time();
}
static void stop_timer() {
    s_running = false;
    // Unsubscribe from tick event; saves battery
    tick_timer_service_unsubscribe();
}

static void toggle_timer() {
    if (s_running) { stop_timer(); }
    else { start_timer(); }
}

static void select_click_handler() {
    toggle_timer();
}

static void click_config_provider(void *context) {
    ButtonId id = BUTTON_ID_SELECT;
    window_single_click_subscribe(id, select_click_handler);
}

static void init() {
    load_settings();

    app_message_register_inbox_received(inbox_received_handler);
    app_message_open(128, 128);

    // Create main Window element and assign to pointer
    s_window = window_create();

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload
    });

    // Set up button click handler
    window_set_click_config_provider(s_window, click_config_provider);

    // Show the Window on the watch, with animated=true
    window_stack_push(s_window, true);
}

static void deinit() {
    if (s_window) window_destroy(s_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
