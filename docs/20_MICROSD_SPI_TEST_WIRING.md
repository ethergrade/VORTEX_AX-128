# 20 — microSD SPI test wiring

This document covers both the user's photographed six-pin microSD breakout and the now-working display's built-in microSD slot. The `firmware/VortexSDTest/` sketch defaults to the built-in display slot; `VORTEX_SD_SOURCE_DISPLAY_SLOT=0` selects the separate breakout. These are **temporary bench assignments**, not frozen final PCB pinouts. The external breakout previously failed at CMD0; the display slot has not been tested yet.

The module's rear silkscreen identifies its pads. With the **rear side facing you, socket on the left, and six pads on the right**, they run **top to bottom** as `3V3`, `CS`, `MOSI`, `CLK`, `MISO`, `GND`. Use the printed labels as the authority if the module is rotated.

Privacy-safe copies of the user's two external-module photos are in [`assets/Modules/`](../assets/Modules/). The display pin map, including the user's confirmed `LCD_RST = GPIO21`, is in [`21_FIRST_SPI_DISPLAY_WIRING.md`](21_FIRST_SPI_DISPLAY_WIRING.md).

| Module pad | Freenove ESP32-S3 | Purpose |
|---|---|---|
| `3V3` (top square pad) | `3V3` | 3.3 V supply; do not connect to `5V` |
| `CS` | `GPIO21` | SPI chip select |
| `MOSI` | `GPIO14` | Data from ESP32 to card |
| `CLK` | `GPIO12` | SPI clock |
| `MISO` | `GPIO13` | Data from card to ESP32 |
| `GND` (bottom pad) | `GND` | Common ground |

The external-breakout assignments above apply only when that breakout is used with the display disconnected. Set `VORTEX_SD_SOURCE_DISPLAY_SLOT` to `0` in `VortexSDTest.ino` for this mode. The photographed breakout is marked `3V3`; no 5 V supply connection is part of this test.

## Test sequence: separate six-pin breakout

1. Disconnect USB power. Add a reliable header or soldered wires to the six module pads; do not rely on loose wires pressed into the holes.
2. Wire each pad by its rear label and check the connections again, especially top `3V3` versus bottom `GND`.
3. On the computer, place [`VORTEX_TEST_440HZ.wav`](../assets/test_samples/VORTEX_TEST_440HZ.wav) in `/samples` on a FAT-formatted microSD card. The sketch does not write to or format the card.
4. Insert the card. In Arduino IDE select `ESP32S3 Dev Module`, `USB Mode: Hardware CDC and JTAG`, `USB CDC On Boot: Disabled`, `Flash Size: 16MB`, `Partition Scheme: 16M Flash (3MB APP / 9.9MB FATFS)`, `CPU Frequency: 240MHz`, `Flash Mode: QIO 80MHz`, `Upload Speed: 921600`, and `PSRAM: OPI PSRAM`; then upload `firmware/VortexSDTest/VortexSDTest.ino`. Set `VORTEX_SD_SOURCE_DISPLAY_SLOT=0` first for this external-breakout mode. These values are also in `firmware/VortexSDTest/sketch.yaml` for Arduino CLI.
5. Open Serial Monitor on the current `/dev/cu.usbmodem...` USB-to-UART port at 115200 baud, then press `RST` once. Keep `USB CDC On Boot` disabled so `Serial` maps to UART0, which is connected to this port. This mode assumes the display is disconnected.
6. Look for card capacity, one compatible PCM16 WAV, and `PSRAM OK` with checksum `49EA2D8F` for the supplied test WAV. Save the serial output for the test log.

For the current persistent mount failure, temporarily change `Tools > Core Debug Level` to `Verbose`, then recompile and upload the same sketch. Capture all serial output: Arduino-ESP32 3.3.12 logs the SD driver's failing command at verbose debug level. `SD.begin` includes both card initialization over SPI and FAT filesystem mounting, so the error may arise in either stage. The volume label is not used by the sketch; the `/samples` directory and WAV are checked only after `SD.begin` succeeds.

The observed verbose failure `Card Failed! cmd: 0x00` with `sdCommand(): no token received` means the card did not answer CMD0 during initial SPI communication. At this stage, verify the actual replacement module's pin labels, supply voltage, ground, CS, SCK, MOSI, MISO and solder continuity. This result occurs before FAT mounting; do not troubleshoot the WAV or `/samples` directory yet.

This diagnostic does not play the WAV; this step has no audio output. Its results are printed only in Serial Monitor. If it prints `microSD non montata`, recheck the six labeled connections with USB power disconnected, ensure the breakout's `3V3` and `GND` are not reversed, verify `CS=GPIO21`, `MOSI=GPIO14`, `CLK=GPIO12`, `MISO=GPIO13`, and confirm the card is inserted and FAT-formatted. Do not change the SPI pin assignments until the wiring is checked.

If the breakout becomes hot or card mounting fails, disconnect power and inspect the pad order and wiring before changing code.

## Alternative: card slot integrated into the display

The display has its own microSD socket. For a test with the working LCD still wired, leave the separate six-pad breakout disconnected. With power disconnected, connect the display header pin labeled `SD_CS` (rear top pin in the supplied orientation; header pin 14) to **GPIO47**. The user confirmed that `LCD_RST` is wired to **GPIO21**, so the integrated-slot mode holds GPIO21 high and selects the card on GPIO47. The test also drives `LCD_CS = GPIO1` high. Neither GPIO21 nor GPIO47 is used by the confirmed keyboard scanner map.

Leave `VORTEX_SD_SOURCE_DISPLAY_SLOT` at `1` (the default), format the card FAT32 and create `/samples`; put the test WAV there. Then upload `firmware/VortexSDTest/` and inspect Serial Monitor for mount, WAV, and PSRAM results. This integrated slot has not yet been tested; the separate breakout's earlier CMD0 failure does not establish whether this reader works.

## Intended role of the card in VORTEX

The card is a removable data/media repository, not the standard firmware boot device. On reset, ESP32-S3's normal boot flow reads the application from the board's internal SPI flash ([Espressif startup flow](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-guides/startup.html)); after startup, firmware can mount the card as FAT storage ([ESP-IDF file-system support](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-guides/file-system-considerations.html)) and read WAV samples, presets, UI assets, and logs. Small critical settings can stay in NVS or have defaults so the instrument can still start if the card is absent. The current test expects `/samples`; future firmware can define additional folders after this card reader is validated.
