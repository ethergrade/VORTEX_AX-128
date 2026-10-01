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

- The user identified that the CD74HC4067 module had initially been connected with VCC and GND reversed after it became very hot. The wiring was corrected before the full-key result was reported. Normal operating temperature after correction was not explicitly confirmed.
- The full 8 × 16 modular scanner was compiled for the ESP32-S3. The Arduino sketch folder was completed with `Keymap.h/.cpp`, `MatrixScanner.h/.cpp`, and `VortexConfig.h`; that exact folder compiled successfully.
- The user reported: “funzionano tutti i tasti, premuto e rilasciato.” This confirms reported press and release events for all 128 physical keys through the mux.
- No Serial Monitor capture or photo of this test was supplied. Physical key-to-coordinate/label correspondence and simultaneous key combinations through the mux were not explicitly confirmed.

Firmware status for this reported scope: **TESTED**. Follow-up checks are listed in `docs/13_NEXT_STEPS.md`.
