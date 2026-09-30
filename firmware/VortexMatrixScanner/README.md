# VortexMatrixScanner

**Status: COMPILED — not yet validated on the physical CD74HC4067 setup.**

This is the active modular keyboard scanner. The first configuration intentionally scans only A1 and A2.

## Wiring for this step

```text
ESP32-S3             CD74HC4067
3V3              ->  VCC
GND              ->  GND
GND              ->  EN
GPIO4            ->  S0
GPIO6            ->  S1
GPIO7            ->  S2
GPIO15           ->  S3
GND through 1kΩ  ->  SIG

CD74HC4067           EK-128 J1
C0               ->  J1-2
C1               ->  J1-4

ESP32-S3             EK-128 J1
GPIO5 INPUT_PULLUP -> J1-3 (Row A)
```

Keep the original EK-128 controller disconnected.

## Expected Serial Monitor output

```text
VORTEX AX-128 - MODULAR MATRIX SCANNER
Rows: 1 | Columns: 2 | Debounce scans: 3
A1 LAYER 1 PRESSED
A1 LAYER 1 RELEASED
A2 LAYER 2 PRESSED
A2 LAYER 2 RELEASED
```

Also test overlapping presses: hold A1, press A2, release A1, then release A2.

## Compile target

- Board: `ESP32S3 Dev Module`
- FQBN: `esp32:esp32:esp32s3`
- CPU: 240 MHz
- Flash: 16 MB
- PSRAM: OPI 8 MB
- USB CDC On Boot: Enabled

## Expansion sequence

1. Confirm A1/A2.
2. Set `kActiveColumnCount = 16` and connect C0–C15.
3. Confirm A1–A16.
4. Connect the remaining seven rows.
5. Set `kActiveRowCount = 8` and validate all 128 keys.

