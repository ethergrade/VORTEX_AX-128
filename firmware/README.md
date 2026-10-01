# Firmware Index

## Historical single-file sketches

| File | Status | Purpose |
|---|---|---|
| `01_memory_test.ino` | TESTED | ESP32-S3 Flash, RAM and PSRAM validation |
| `02_A1_direct_test.ino` | TESTED | First direct A1 key test |
| `03_A1_A2_direct_scan_tested.ino` | TESTED | Direct two-column scan, including overlapping presses |
| `04_CD74HC4067_A1_A2_test.ino` | UNTESTED | Minimal multiplexer test retained as a simple diagnostic |
| `05_full_matrix_scanner_skeleton_UNTESTED.ino` | UNTESTED | Early full-matrix draft |

## Active prototype

`VortexMatrixScanner/` is the active modular C/C++ implementation for the CD74HC4067 keyboard scanner.

Current test configuration:

- all eight rows and sixteen columns;
- 20 ms debounce;
- serial events include matrix coordinate, VORTEX function name, J1 pins, ESP32 row GPIO, and mux channel;
- a running count of distinct pressed keys, plus commands to list missing keys or print the complete map.

The user reported press and release events for all 128 keys through the CD74HC4067 on 2026-10-02. Physical key-to-coordinate labels, overlapping presses through the mux, and normal mux temperature after correcting reversed VCC/GND remain to be checked. See `docs/12_TEST_LOG_AND_MILESTONES.md`.
