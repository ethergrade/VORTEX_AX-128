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
- First hardware attempt: the application serial output now works, but `SD.begin` reports `microSD non montata`. No successful card mount, WAV enumeration, or PSRAM copy is claimed yet.
- The initial empty output was first attributed incorrectly to CDC being disabled. Hardware inspection later identified the selected `/dev/cu.usbmodem...` device as the board's `USB Single Serial` USB-to-UART bridge (vendor `0x1A86`); the boot ROM output arrived over UART0. With `USB CDC On Boot` enabled, Arduino core 3.3.12 maps `Serial` to the ESP32-S3's separate native USB Serial/JTAG peripheral, so application messages went to an interface that was not connected to this port. The sketch's `if (Serial)` gate was also removed, but that change alone did not fix the routing.
- Set `USB CDC On Boot: Disabled` for this bridge, which maps `Serial` to UART0. Compiled and uploaded through Arduino IDE 2.3.10 with ESP32 core 3.3.12; the IDE reported 364,764 program bytes and 22,880 global bytes. The application now prints the VORTEX test banner and SPI pin map. The current hardware result is `microSD non montata`; the serial path is working, but card mounting, WAV enumeration, and PSRAM loading still need a successful test.
- Because the first card attempt may have shared the SPI bus with the display, added an explicit inactive-high `LCD_CS = GPIO1` in the SD test before initializing SPI. Recompiled and uploaded with CDC disabled (364,820 program bytes; 22,880 global bytes). The monitor reports `SD_CS GPIO21; LCD_CS GPIO1 disattivato`, followed by `microSD non montata`. This code change did not resolve card mounting; the physical card, power, SPI wiring, and FAT format still need checking.
- Updated the saved Arduino CLI FQBN and the SD, display, and keyboard sketch instructions to keep CDC disabled when using this USB-to-UART port. The SD sketch still waits 1.5 seconds after reset before diagnostics. No audio is expected because this test reads the WAV but does not play it.
- Saved the full board FQBN/options in `firmware/VortexSDTest/sketch.yaml` and the corresponding IDE menu values in its README. Arduino CLI can use the file's `default_fqbn`; Arduino IDE's board menus must be selected manually, since this file is not an IDE profile menu.
- Follow-up hardware report: the user checked the wiring, formatted the card as FAT32 on macOS, labeled the volume `VAULT`, and placed the supplied WAV in the card root (without a `/samples` directory). The user then soldered a replacement SD module; the same mount error remains. The exact latest serial capture and a photo of the replacement module/pin labels have not yet been provided. The root-vs-`/samples` location affects WAV discovery only after a successful mount; it cannot cause `SD.begin` to report `microSD non montata`.
- Next isolation test: disconnect power, temporarily remove the display module from the SPI bus (including its power and SPI signal wires), and test the external SD breakout alone. Record the complete serial output and a photo showing the replacement breakout's labels and wiring before changing GPIO assignments.
- Isolation result: the user disconnected the display and retried the external microSD test. The serial output remains `microSD non montata` after the banner and the same GPIO report. This confirms the failure persists with the display removed; the exact replacement-breakout pin labels remain unverified from a photo.
- Next firmware diagnostic: temporarily set Arduino IDE `Tools > Core Debug Level > Verbose`, recompile/upload the same SD sketch, and capture all serial output. Arduino-ESP32 3.3.12's SD driver emits command-level warnings/errors at this level, which should distinguish no SPI response from a card initialization or filesystem error. The library already switches to 400 kHz during card initialization, so changing the sketch's 4 MHz operating frequency is not the next diagnostic.
- The user supplied a screenshot with `Core Debug Level: Verbose` selected and a new serial capture; the capture still contains only the boot banner, application banner/pin map, and generic `microSD non montata` line. The screenshot does not show whether the sketch was uploaded after changing the compile-time debug option. Next step is to upload with Verbose selected (press Upload, not only `RST`) and check for SD-driver warnings before adding a raw SPI probe.
- The user then shared the ESP32 core's Verbose startup report: internal heap and 8 MB PSRAM are present, and GPIO 12/13/14 are assigned `SPI_MASTER_SCK/MISO/MOSI[0]` while GPIO21 is assigned `SD_SS`. This confirms the firmware's pin assignments, not the physical wiring or the card response. The supplied excerpt does not include the application banner, SD-driver warnings, or final `SD.begin` result, so the next diagnostic still requires the complete serial capture from the VORTEX banner through the SD result.
- Read the earlier lines from the open Arduino Serial Monitor after the user repeated the tail excerpt. The verbose SD driver reports several `sdCommand(): no token received` warnings and `Card Failed! cmd: 0x00` (CMD0 / GO_IDLE_STATE). The card therefore is not answering the initial SPI command; filesystem format, volume label, and WAV path are not yet involved. The driver log confirms software configuration of SCK=GPIO12, MISO=GPIO13, MOSI=GPIO14 and SD_SS=GPIO21. Physical wiring and the replacement module's exact pinout/supply remain unverified.

## 2026-10-02 — first SPI display diagnostic prepared

- The user supplied a rear photo of a 14-pin SPI TFT with capacitive-touch flex and onboard microSD socket. The printed labels and component layout match the LCDWiki MSP3526/ST7796 board family; exact front model/size and controller remain to be confirmed.
- Prepared the separate `firmware/VortexDisplayTest/` sketch, using the previously selected `GPIO12`/`13`/`14` SPI wires and separate `LCD_CS = GPIO1`, `LCD_RS = GPIO38`, `LCD_RST = GPIO47`. It uses only `SPI.h`, sends the manufacturer's ST7796 initialization sequence, displays RGB colour bands and blinks a white block.
- The exact Arduino sketchbook copy also compiled for `ESP32S3 Dev Module`, 16 MB flash, OPI PSRAM and USB CDC with Arduino ESP32 core 3.3.12: 308,244 bytes of program storage and 22,880 bytes of global variables. The user has not yet reported a powered display result.
- The manufacturer's schematic indicates that touch I²C pins may rise to the display's 5 V `VCC`; all `CTP_*` pins, `LED` and display-slot `SD_CS` are intentionally left disconnected in this first test.
- Privacy-safe copies of the user's two microSD photos and display rear photo were added to the project assets after location metadata was removed and checked.
- Firmware status: **COMPILED**. Hardware status: **UNTESTED**. The microSD diagnostic is also still awaiting its physical test.

## 2026-10-02 — display test resumed

- The user chose to pause microSD troubleshooting and test the SPI display first.
- Opened the separate `VortexDisplayTest` sketch from the Arduino sketchbook copy and compiled it successfully with the current ESP32-S3 options: 338,580 program bytes and 22,824 global bytes. No upload or physical display result is claimed yet.
- Next: with USB power disconnected, wire only display power and LCD SPI/control signals per `docs/21_FIRST_SPI_DISPLAY_WIRING.md`; leave touch and the display's own SD slot disconnected, and keep the external SD breakout out of this test. Upload the LCD sketch and report the screen pattern plus serial output.
- Firmware status: **COMPILED**. Display hardware status: **UNTESTED**. The external microSD remains paused at the CMD0 no-response finding.
