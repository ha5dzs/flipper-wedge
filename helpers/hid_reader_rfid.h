#pragma once

#include <furi.h>
#include <lfrfid/lfrfid_worker.h>

#define HID_READER_RFID_UID_MAX_LEN 8

typedef struct HidReaderRfid HidReaderRfid;

typedef struct {
    uint8_t uid[HID_READER_RFID_UID_MAX_LEN];
    uint8_t uid_len;
    char protocol_name[32];
} HidReaderRfidData;

typedef void (*HidReaderRfidCallback)(HidReaderRfidData* data, void* context);

/** Allocate RFID reader
 *
 * @return HidReaderRfid instance
 */
HidReaderRfid* hid_reader_rfid_alloc(void);

/** Free RFID reader
 *
 * @param instance HidReaderRfid instance
 */
void hid_reader_rfid_free(HidReaderRfid* instance);

/** Set callback for tag detection
 *
 * @param instance HidReaderRfid instance
 * @param callback Callback function
 * @param context Callback context
 */
void hid_reader_rfid_set_callback(
    HidReaderRfid* instance,
    HidReaderRfidCallback callback,
    void* context);

/** Start RFID scanning
 *
 * @param instance HidReaderRfid instance
 */
void hid_reader_rfid_start(HidReaderRfid* instance);

/** Stop RFID scanning
 *
 * @param instance HidReaderRfid instance
 */
void hid_reader_rfid_stop(HidReaderRfid* instance);

/** Check if RFID is currently scanning
 *
 * @param instance HidReaderRfid instance
 * @return true if scanning
 */
bool hid_reader_rfid_is_scanning(HidReaderRfid* instance);
