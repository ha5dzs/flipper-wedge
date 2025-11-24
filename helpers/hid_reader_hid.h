#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_usb.h>
#include <furi_hal_usb_hid.h>
#include <bt/bt_service/bt.h>
#include <extra_profiles/hid_profile.h>

#define HID_READER_BT_KEYS_STORAGE_NAME ".hid_reader_bt.keys"

typedef struct HidReaderHid HidReaderHid;

typedef void (*HidReaderHidConnectionCallback)(bool usb_connected, bool bt_connected, void* context);

/** Allocate HID helper
 *
 * @return HidReaderHid instance
 */
HidReaderHid* hid_reader_hid_alloc(void);

/** Free HID helper
 *
 * @param instance HidReaderHid instance
 */
void hid_reader_hid_free(HidReaderHid* instance);

/** Start HID services (USB and/or BT)
 *
 * @param instance HidReaderHid instance
 * @param enable_usb Enable USB HID (disable for serial debugging)
 * @param enable_bt Enable Bluetooth HID
 */
void hid_reader_hid_start(HidReaderHid* instance, bool enable_usb, bool enable_bt);

/** Stop HID services
 *
 * @param instance HidReaderHid instance
 */
void hid_reader_hid_stop(HidReaderHid* instance);

/** Set connection status callback
 *
 * @param instance HidReaderHid instance
 * @param callback Callback function
 * @param context Callback context
 */
void hid_reader_hid_set_connection_callback(
    HidReaderHid* instance,
    HidReaderHidConnectionCallback callback,
    void* context);

/** Check if USB HID is connected
 *
 * @param instance HidReaderHid instance
 * @return true if connected
 */
bool hid_reader_hid_is_usb_connected(HidReaderHid* instance);

/** Check if BT HID is connected
 *
 * @param instance HidReaderHid instance
 * @return true if connected
 */
bool hid_reader_hid_is_bt_connected(HidReaderHid* instance);

/** Check if any HID connection is available
 *
 * @param instance HidReaderHid instance
 * @return true if USB or BT is connected
 */
bool hid_reader_hid_is_connected(HidReaderHid* instance);

/** Type a string via HID keyboard
 * Sends to both USB and BT if connected
 *
 * @param instance HidReaderHid instance
 * @param str String to type
 */
void hid_reader_hid_type_string(HidReaderHid* instance, const char* str);

/** Type a single character via HID keyboard
 *
 * @param instance HidReaderHid instance
 * @param c Character to type
 */
void hid_reader_hid_type_char(HidReaderHid* instance, char c);

/** Press and release Enter key
 *
 * @param instance HidReaderHid instance
 */
void hid_reader_hid_press_enter(HidReaderHid* instance);

/** Release all keys
 *
 * @param instance HidReaderHid instance
 */
void hid_reader_hid_release_all(HidReaderHid* instance);
