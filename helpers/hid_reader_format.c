#include "hid_reader_format.h"
#include <stdio.h>
#include <string.h>

void hid_reader_format_uid(
    const uint8_t* uid,
    uint8_t uid_len,
    const char* delimiter,
    char* output,
    size_t output_size) {

    if(!uid || uid_len == 0 || !output || output_size == 0) {
        if(output && output_size > 0) {
            output[0] = '\0';
        }
        return;
    }

    size_t pos = 0;
    size_t delim_len = delimiter ? strlen(delimiter) : 0;

    for(uint8_t i = 0; i < uid_len && pos < output_size - 3; i++) {
        // Add delimiter before byte (except first)
        if(i > 0 && delim_len > 0) {
            if(pos + delim_len >= output_size - 3) break;
            memcpy(output + pos, delimiter, delim_len);
            pos += delim_len;
        }

        // Add hex byte (uppercase)
        if(pos + 2 >= output_size) break;
        snprintf(output + pos, output_size - pos, "%02X", uid[i]);
        pos += 2;
    }

    output[pos] = '\0';
}

void hid_reader_format_output(
    const uint8_t* nfc_uid,
    uint8_t nfc_uid_len,
    const uint8_t* rfid_uid,
    uint8_t rfid_uid_len,
    const char* ndef_text,
    const char* delimiter,
    bool nfc_first,
    char* output,
    size_t output_size) {

    if(!output || output_size == 0) {
        return;
    }

    output[0] = '\0';
    size_t pos = 0;

    char uid_buf[64];

    if(nfc_first) {
        // NFC UID first
        if(nfc_uid && nfc_uid_len > 0) {
            hid_reader_format_uid(nfc_uid, nfc_uid_len, delimiter, uid_buf, sizeof(uid_buf));
            size_t len = strlen(uid_buf);
            if(pos + len < output_size) {
                memcpy(output + pos, uid_buf, len);
                pos += len;
            }

            // Append NDEF text directly after NFC UID (no delimiter)
            if(ndef_text && ndef_text[0] != '\0') {
                size_t ndef_len = strlen(ndef_text);
                if(pos + ndef_len < output_size) {
                    memcpy(output + pos, ndef_text, ndef_len);
                    pos += ndef_len;
                }
            }
        }

        // Then RFID UID
        if(rfid_uid && rfid_uid_len > 0) {
            hid_reader_format_uid(rfid_uid, rfid_uid_len, delimiter, uid_buf, sizeof(uid_buf));
            size_t len = strlen(uid_buf);
            if(pos + len < output_size) {
                memcpy(output + pos, uid_buf, len);
                pos += len;
            }
        }
    } else {
        // RFID UID first
        if(rfid_uid && rfid_uid_len > 0) {
            hid_reader_format_uid(rfid_uid, rfid_uid_len, delimiter, uid_buf, sizeof(uid_buf));
            size_t len = strlen(uid_buf);
            if(pos + len < output_size) {
                memcpy(output + pos, uid_buf, len);
                pos += len;
            }
        }

        // Then NFC UID
        if(nfc_uid && nfc_uid_len > 0) {
            hid_reader_format_uid(nfc_uid, nfc_uid_len, delimiter, uid_buf, sizeof(uid_buf));
            size_t len = strlen(uid_buf);
            if(pos + len < output_size) {
                memcpy(output + pos, uid_buf, len);
                pos += len;
            }

            // Append NDEF text directly after NFC UID (no delimiter)
            if(ndef_text && ndef_text[0] != '\0') {
                size_t ndef_len = strlen(ndef_text);
                if(pos + ndef_len < output_size) {
                    memcpy(output + pos, ndef_text, ndef_len);
                    pos += ndef_len;
                }
            }
        }
    }

    output[pos] = '\0';
}
