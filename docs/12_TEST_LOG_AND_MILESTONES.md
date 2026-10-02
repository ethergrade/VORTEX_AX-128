# 12 — Test Log and Milestones

## 2026-09-28 — ESP32 development environment

Arduino IDE on macOS configured successfully.

Board:

`ESP32S3 Dev Module`

Upload confirmed over USB-C.

esptool identification:

- ESP32-S3 QFN56 rev v0.2
- 240 MHz
- Wi-Fi
- Bluetooth LE
- embedded PSRAM 8 MB

Runtime test:

```text
CPU: 240 MHz
Flash: 16 MB
PSRAM total: 8 MB
PSRAM free: ~8188 KB at startup
Internal free RAM: ~348 KB
PSRAM: OK
```

## EK-128 matrix discovery

Confirmed 8 rows and 16 columns.

Rows:

```text
A=3
B=5
C=7
D=9
E=11
F=13
G=15
H=17
```

Columns:

```text
1=2
2=4
...
15=30
16=32
```

Diode drop observed around:

```text
0.543–0.546 V
```

## First key

A1 successfully detected by ESP32.

Serial output:

```text
A1 PRESSED
A1 RELEASED
```

Breakout visual behaviour:

- active column GPIO LED LOW/off;
- row input HIGH/on at idle;
- pressing key pulls row LOW/off.

## Two-key scan

A1 and A2 successfully scanned independently.

Overlapping presses successfully detected.

This validates:

- matrix mapping;
- diode orientation;
- active-low scan;
- simultaneous key handling;
- event detection.

## Planned next test at that stage

Validate CD74HC4067 column scan using:

- C0 -> J1-2
- C1 -> J1-4
- Row A J1-3 -> GPIO5
- S0..S3 frozen GPIOs
- SIG -> 1 kΩ -> GND
- EN -> GND
- VCC -> 3.3 V

## 2026-09-30 — repository and modular scanner preparation

- Consolidated project documentation, privacy-safe photos and previous sketches in Git.
- Added a modular C/C++ scanner with external configuration and keymap files.
- Configured the active build for Row A and columns C0/C1 only.
- Added three-scan debounce without dynamic memory allocation.

Compile result with Arduino ESP32 core 3.3.12 and the ESP32S3 Dev Module target:

```text
Program storage: 304,888 bytes / 3,145,728 bytes (9%)
Global variables: 23,192 bytes / 327,680 bytes (7%)
```

Software status: **COMPILED**. Hardware status remains **UNTESTED** until the CD74HC4067 bench test is completed.

## 2026-10-02 — complete EK-128 scan, user-reported bench result

- The user identified that the CD74HC4067 module had initially been connected with VCC and GND reversed after it became very hot. The wiring was corrected before the full-key result was reported.
- The full 8 × 16 modular scanner was compiled for the ESP32-S3. The Arduino sketch folder was completed with `Keymap.h/.cpp`, `MatrixScanner.h/.cpp`, and `VortexConfig.h`; that exact folder compiled successfully.
- The user reported: “funzionano tutti i tasti, premuto e rilasciato.” This confirms reported press and release events for all 128 physical keys through the mux.
- The user subsequently confirmed exact physical key-to-coordinate correspondence, working multiple simultaneous presses, and normal multiplexer temperature after the wiring correction.
- No Serial Monitor capture, photo, temperature measurement, or exhaustive key-combination matrix was supplied; the above is a user-reported bench result.

Firmware status for this reported scope: **TESTED**. The keyboard milestone is complete; microSD validation is next in `docs/13_NEXT_STEPS.md`.

## 2026-10-02 — microSD diagnostic prepared

- Added a separate SPI microSD sketch to detect the card, list `/samples`, inspect PCM16 WAV files, and copy up to 64 KiB of sample data into PSRAM.
- Generated a 1-second 44.1 kHz mono PCM16 test WAV for the card.
- The sketch compiled for `ESP32S3 Dev Module` with the installed Arduino ESP32 core 3.3.12.
- The user's module photos identify a six-pad breakout labeled `3V3`, `CS`, `MOSI`, `CLK`, `MISO`, `GND`. Temporary signal assignments and the 3.3 V supply connection are recorded in `docs/20_MICROSD_SPI_TEST_WIRING.md`.
- Hardware status: **UNTESTED**. No wiring, card mount, WAV enumeration, or PSRAM copy result is claimed yet.
- The user later reported a successful upload followed by an empty output. The Arduino build metadata for that upload records `CDCOnBoot=default` (disabled), so the USB Serial Monitor was not configured to receive the sketch's `Serial` diagnostics. The user has not yet rerun with CDC enabled; card detection and WAV loading remain **UNTESTED**. No sound is expected because this diagnostic does not play the WAV.
- The user enabled USB CDC and then reported only garbled characters followed by the ESP32-S3 ROM boot log; no application diagnostic was captured. The latest build metadata confirms `CDCOnBoot=cdc`, `USBMode=hwcdc`, `PSRAM=opi` and the 16 MB / 3 MB-app FATFS settings. Updated the sketch to wait for a Serial Monitor connection before starting the read-only SD test, so a USB re-enumeration cannot consume the one-shot output. The updated sketch compiled from the Arduino sketchbook copy with Arduino ESP32 core 3.3.12: 360,120 bytes of program storage and 23,072 bytes of globals. It still needs to be uploaded and the test rerun; physical SD status remains **UNTESTED**.
- Saved the full board FQBN/options in `firmware/VortexSDTest/sketch.yaml` and the corresponding IDE menu values in its README. Arduino CLI can use the file's `default_fqbn`; Arduino IDE's board menus must be selected manually, since this file is not an IDE profile menu.

## 2026-10-02 — first SPI display diagnostic prepared

- The user supplied a rear photo of a 14-pin SPI TFT with capacitive-touch flex and onboard microSD socket. The printed labels and component layout match the LCDWiki MSP3526/ST7796 board family; exact front model/size and controller remain to be confirmed.
- Prepared the separate `firmware/VortexDisplayTest/` sketch, using the previously selected `GPIO12`/`13`/`14` SPI wires and separate `LCD_CS = GPIO1`, `LCD_RS = GPIO38`, `LCD_RST = GPIO47`. It uses only `SPI.h`, sends the manufacturer's ST7796 initialization sequence, displays RGB colour bands and blinks a white block.
- The exact Arduino sketchbook copy also compiled for `ESP32S3 Dev Module`, 16 MB flash, OPI PSRAM and USB CDC with Arduino ESP32 core 3.3.12: 308,244 bytes of program storage and 22,880 bytes of global variables. The user has not yet reported a powered display result.
- The manufacturer's schematic indicates that touch I²C pins may rise to the display's 5 V `VCC`; all `CTP_*` pins, `LED` and display-slot `SD_CS` are intentionally left disconnected in this first test.
- Privacy-safe copies of the user's two microSD photos and display rear photo were added to the project assets after location metadata was removed and checked.
- Firmware status: **COMPILED**. Hardware status: **UNTESTED**. The microSD diagnostic is also still awaiting its physical test.
