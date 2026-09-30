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

## Current next test

Validate CD74HC4067 column scan using:

- C0 -> J1-2
- C1 -> J1-4
- Row A J1-3 -> GPIO5
- S0..S3 frozen GPIOs
- SIG -> 1 kΩ -> GND
- EN -> GND
- VCC -> 3.3 V
