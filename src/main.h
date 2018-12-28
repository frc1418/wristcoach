#pragma once
#include <pebble.h>

#define SETTINGS_KEY 1

// A structure containing our settings
typedef struct ClaySettings {
    int EarlyWarningTime;
    int EndgameWarningTime;
} __attribute__((__packed__)) ClaySettings;

// TODO: define functions here
