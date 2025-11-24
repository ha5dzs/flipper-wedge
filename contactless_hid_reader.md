# Contactless HID Reader — Full Development Guidelines

A complete technical specification for implementing a Flipper Zero app that:

- Reads **RFID UIDs**
- Reads **NFC UIDs**
- Reads optional **NDEF text records**
- Combines or orders them based on "mode"
- Outputs them **as typed HID keyboard input** (USB + Bluetooth HID)
- Provides a configurable UI and an advanced options panel
- Uses timeouts to avoid spamming and keep login behavior clean
- Never stores any history
- Works seamlessly with USB & BLE
- Resets partial scans when needed

This document is structured for the **leedave/flipper-zero-fap-boilerplate**.

---

## 1. Project Overview

**App Name:** `Contactless HID Reader`  
**App Type:** Flipper Zero FAP  
**Framework:** leedave/flipper-zero-fap-boilerplate  
**Primary Function:**  
Read contactless tag UIDs and type them out over HID using a configurable mode.

This app turns the Flipper into a **token-based keyboard authenticator** — scanning a tag types out a login code or credential.

---

## 2. Supported Input Technologies

### RFID (125 kHz)
Use Flipper’s built-in reader; support all built-in types:

- EM4100 / EM4x0x
- HID Prox
- Indala (where UID readable)
- Others supported by Flipper firmware

### NFC (13.56 MHz)
Support all native Flipper NFC readers:

- ISO14443A (MIFARE, NTAG, Ultralight, etc.)
- ISO14443B (UID only)
- NFC Forum Type 1–5
- NDEF parsing for **text records only**

---

## 3. Output Format

### UID Conversion
- Convert UID bytes → ASCII hex characters  
- Join bytes with user-selected delimiter  
- Default delimiter: empty string `""`
- No `UID:` prefix  
- No extra whitespace  
- Optional `<ENTER>` appended (Advanced setting)

**Example:**  
Bytes: `FE 90 1A B3`  
Delimiter: `:`  
Output: `FE:90:1A:B3`

### NDEF (Optional)
- Only use **NDEF text records**
- Respect language codes
- Append **directly after the NFC UID** (no delimiter)
- Skip NDEF entirely if none present

---

## 4. HID Output Rules

1. Output via **Bluetooth HID** if paired
2. Output via **USB HID** if connected
3. If both are connected → send to *both*
4. If neither connected:
   - Show: **“Connect USB or Bluetooth HID”**
   - Disable scanning

### Advanced Settings
- Delimiter string
- Append Enter (ON/OFF)
- Enable NDEF parsing (ON/OFF)

---

## 5. Modes

Home screen modes:

1. **NFC**
2. **RFID**
3. **NFC → RFID**
4. **RFID → NFC**
5. **Scan Order**
6. **Pair Bluetooth**

**Default on app launch:** NFC → RFID  
**Scanning auto-starts** if HID connection exists.

---

## 6. Mode Behavior Details

### 1. NFC Mode
- Read NFC → output:
<NFC_UID>[<NDEF_payload>]
- Show result 200ms → “Sent”
- Reset

### 2. RFID Mode
- Read RFID → output:
<RFID_UID>
- Show 200ms → “Sent”
- Reset

---

### 3. NFC → RFID Mode
Sequence:

1. Scan NFC → store UID  
2. Start *200ms timer*  
3. If RFID scanned in time:
NFC_UID [+ NDEF] RFID_UID
4. If RFID not scanned:
- Reset
- Show “Tag Not Present”

---

### 4. RFID → NFC Mode
Reverse ordering:

1. Scan RFID  
2. Wait up to 200ms for NFC  
3. If both:
RFID_UID NFC_UID [+ NDEF]
4. Else reset with “Tag Not Present”

---

### 5. Scan Order Mode
- Poll NFC & RFID simultaneously  
- Whatever is detected first → output immediately  
- No combination logic  
- Show result 200ms → “Sent”

---

### 6. Pair Bluetooth
- Launch Flipper's built-in BT HID pairing flow

---

## 7. Scan Logic & Timeouts

### Display Timing
- Show scanned UID(s) for 200ms
- Then show “Sent”
- Return to idle and enable scanning again

### Partial Scan Timeout
- If part 1 of a 2-part mode is scanned:
- Start 200ms timer
- If part 2 does not arrive → reset state, show “Tag Not Present”

### Scan Debounce
- Require tag removal before another scan
- Add a cooldown of 200–300ms after each output

---

## 8. User Interface Layout

### Home Screen
- **Title:** Contactless HID Reader  
- **Status line:** HID connection states  
- **Mode selector list**  
- **Bottom menu:** “Advanced”

During scanning:
- Replace content with UID(s)
- After 200ms → “Sent”

---

### Advanced Screen
Options:
1. **Delimiter** (default: empty)
2. **Append Enter** (ON/OFF)
3. **Enable NDEF Parsing** (ON/OFF)
4. **Scan NDEF Record**

---

### NDEF Inspector
1. Prompt: “Scan NFC Tag…”  
2. Read all NDEF text records  
3. List with:
- Type
- Language code
- Payload preview  
4. Selecting one sets it as the active NDEF payload.

---

## 9. Recommended Module Structure

/src
/scenes
home_scene.c
advanced_scene.c
ndef_scan_scene.c
mode_selection_scene.c
/logic
nfc_reader.c
rfid_reader.c
combined_modes.c
hid_output.c
timeout_manager.c
ndef_parser.c
/ui
render_helpers.c
list_view_helpers.c
app.c
config.c

---

## 10. Event Loop & Polling

### Polling Behavior
- Poll both NFC & RFID inside main loop
- In combo modes: activate second listener only after first scan
- In Scan Order mode: alternate NFC/RFID polling every ~10–15ms

### Duty Cycling
Alternate calls like:
tick 1: NFC poll
tick 2: RFID poll
tick 3: NFC poll
...

---

## 11. HID Output Pipeline

1. Format UID(s) using delimiter
2. Append NDEF payload if applicable
3. Send via Bluetooth HID (if connected)
4. Send via USB HID (if connected)
5. Append `<ENTER>` if selected
6. Show “Sent”
7. Start cooldown

---

## 12. Edge Case Handling

### Tag left on reader
Ignore until removed.

### Rapid scans
Cooldown prevents spam.

### No readable NDEF
Silently skip.

### No HID connection
Disable scanning and show message.

---

## 13. Security Notes

- No scan history stored
- No persistent caching of tags
- No auto-start without HID path
- No PIN unlock required

---

## 14. Future Extensibility

Optional future features:

- Custom format templates
- Multi-record NDEF concatenation
- Tag whitelists/blacklists
- CRC / hashing output
- Master-tag unlock mode

---

# End of Document

