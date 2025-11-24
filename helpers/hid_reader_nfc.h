#pragma once

#include <furi.h>
#include <nfc/nfc.h>
#include <nfc/nfc_poller.h>
#include <nfc/nfc_scanner.h>
#include <nfc/protocols/iso14443_3a/iso14443_3a.h>
#include <nfc/protocols/iso14443_3a/iso14443_3a_poller.h>
#include <nfc/protocols/mf_ultralight/mf_ultralight.h>
#include <nfc/protocols/mf_ultralight/mf_ultralight_poller.h>

#define HID_READER_NFC_UID_MAX_LEN 10
#define HID_READER_NDEF_MAX_LEN 256

typedef struct HidReaderNfc HidReaderNfc;

typedef struct {
    uint8_t uid[HID_READER_NFC_UID_MAX_LEN];
    uint8_t uid_len;
    char ndef_text[HID_READER_NDEF_MAX_LEN];
    bool has_ndef;
} HidReaderNfcData;

typedef void (*HidReaderNfcCallback)(HidReaderNfcData* data, void* context);

/** Allocate NFC reader
 *
 * @return HidReaderNfc instance
 */
HidReaderNfc* hid_reader_nfc_alloc(void);

/** Free NFC reader
 *
 * @param instance HidReaderNfc instance
 */
void hid_reader_nfc_free(HidReaderNfc* instance);

/** Set callback for tag detection
 *
 * @param instance HidReaderNfc instance
 * @param callback Callback function
 * @param context Callback context
 */
void hid_reader_nfc_set_callback(
    HidReaderNfc* instance,
    HidReaderNfcCallback callback,
    void* context);

/** Start NFC scanning
 *
 * @param instance HidReaderNfc instance
 * @param parse_ndef Whether to parse NDEF records
 */
void hid_reader_nfc_start(HidReaderNfc* instance, bool parse_ndef);

/** Stop NFC scanning
 *
 * @param instance HidReaderNfc instance
 */
void hid_reader_nfc_stop(HidReaderNfc* instance);

/** Check if NFC is currently scanning
 *
 * @param instance HidReaderNfc instance
 * @return true if scanning
 */
bool hid_reader_nfc_is_scanning(HidReaderNfc* instance);

/** Process NFC state machine from main thread
 * Call this in the tick event handler to safely process NFC events
 *
 * @param instance HidReaderNfc instance
 * @return true if a tag was successfully read
 */
bool hid_reader_nfc_tick(HidReaderNfc* instance);
