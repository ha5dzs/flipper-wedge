#pragma once

#include <stdlib.h>
#include <string.h>
#include <storage/storage.h>
#include <flipper_format/flipper_format_i.h>
#include "../hid_reader.h"

#define HID_READER_SETTINGS_FILE_VERSION 1
#define CONFIG_FILE_DIRECTORY_PATH EXT_PATH("apps_data/hid_reader")
#define HID_READER_SETTINGS_SAVE_PATH CONFIG_FILE_DIRECTORY_PATH "/hid_reader.conf"
#define HID_READER_SETTINGS_SAVE_PATH_TMP HID_READER_SETTINGS_SAVE_PATH ".tmp"
#define HID_READER_SETTINGS_HEADER "HidReader Config File"
#define HID_READER_SETTINGS_KEY_HAPTIC "Haptic"
#define HID_READER_SETTINGS_KEY_LED "Led"
#define HID_READER_SETTINGS_KEY_SPEAKER "Speaker"
#define HID_READER_SETTINGS_KEY_SAVE_SETTINGS "SaveSettings"

void hid_reader_save_settings(void* context);
void hid_reader_read_settings(void* context);