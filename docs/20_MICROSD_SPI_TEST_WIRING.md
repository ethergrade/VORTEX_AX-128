# 20 — microSD SPI test wiring

This wiring applies to the user's photographed six-pin microSD breakout and the separate `firmware/VortexSDTest/` sketch. It is a **temporary bench assignment**, not a frozen final PCB pinout. The breakout and card have not yet been tested electrically.

The module's rear silkscreen identifies its pads. With the **rear side facing you, socket on the left, and six pads on the right**, they run **top to bottom** as `3V3`, `CS`, `MOSI`, `CLK`, `MISO`, `GND`. Use the printed labels as the authority if the module is rotated.

Privacy-safe copies of the user's two module photos are in [`assets/Modules/`](../assets/Modules/). The separate first-display smoke test reuses `GPIO12`/`13`/`14` for SPI but assigns the display its own `LCD_CS = GPIO1`; see [`21_FIRST_SPI_DISPLAY_WIRING.md`](21_FIRST_SPI_DISPLAY_WIRING.md). The display module's supply is different from this breakout's `3V3` supply.

| Module pad | Freenove ESP32-S3 | Purpose |
|---|---|---|
| `3V3` (top square pad) | `3V3` | 3.3 V supply; do not connect to `5V` |
| `CS` | `GPIO21` | SPI chip select |
| `MOSI` | `GPIO14` | Data from ESP32 to card |
| `CLK` | `GPIO12` | SPI clock |
| `MISO` | `GPIO13` | Data from card to ESP32 |
| `GND` (bottom pad) | `GND` | Common ground |

These four GPIOs are exposed on the documented Freenove board and do not overlap the EK-128 scanner's existing GPIOs. The keyboard scanner sketch is left intact; `VortexSDTest` is uploaded separately for this bench test. The photographed breakout is marked `3V3`; no 5 V supply connection is part of this test.

## Test sequence

1. Disconnect USB power. Add a reliable header or soldered wires to the six module pads; do not rely on loose wires pressed into the holes.
2. Wire each pad by its rear label and check the connections again, especially top `3V3` versus bottom `GND`.
3. On the computer, place [`VORTEX_TEST_440HZ.wav`](../assets/test_samples/VORTEX_TEST_440HZ.wav) in `/samples` on a FAT-formatted microSD card. The sketch does not write to or format the card.
4. Insert the card. In Arduino IDE select `ESP32S3 Dev Module`, `USB Mode: Hardware CDC and JTAG`, `USB CDC On Boot: Enabled`, `Flash Size: 16MB`, `Partition Scheme: 16M Flash (3MB APP / 9.9MB FATFS)`, `CPU Frequency: 240MHz`, `Flash Mode: QIO 80MHz`, `Upload Speed: 921600`, and `PSRAM: OPI PSRAM`; then upload `firmware/VortexSDTest/VortexSDTest.ino`. These values are also in `firmware/VortexSDTest/sketch.yaml` for Arduino CLI.
5. Open Serial Monitor on the current `/dev/cu.usbmodem...` port at 115200 baud. The sketch waits for the Monitor connection before running the test; if it is already open, press `RST` once after upload.
6. Look for card capacity, one compatible PCM16 WAV, and `PSRAM OK` with checksum `49EA2D8F` for the supplied test WAV. Save the serial output for the test log.

This diagnostic does not play the WAV; this step has no audio output. Its results are printed only in Serial Monitor. If the monitor stays empty, verify the full board configuration above, the current USB port and 115200 baud, then close/reopen the Monitor or press `RST`.

If the breakout becomes hot or card mounting fails, disconnect power and inspect the pad order and wiring before changing code.
