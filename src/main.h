#pragma once
#include <pebble.h>

#define SETTINGS_KEY 1

// A structure containing our settings
typedef struct ClaySettings {
    int EarlyWarningTime;
    int EndgameWarningTime;
} __attribute__((__packed__)) ClaySettings;

static void start_timer();
static void stop_timer();
static void prv_default_settings();
static void prv_save_settings();
static void prv_load_settings();
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context);
static void window_load(Window *window);
static void window_unload(Window *window);
static void update_time();
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
static void start_timer();
static void stop_timer();
static void toggle_timer();
static void select_click_handler();
static void click_config_provider(void *context);
static void prv_init();
static void prv_deinit();
int main(void);
