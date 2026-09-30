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

Current configuration:

- Row A only;
- columns C0 and C1 only;
- three-scan debounce;
- serial events include matrix coordinate and VORTEX function name.

Expand `kActiveColumnCount` and `kActiveRowCount` in `VortexMatrixScanner/VortexConfig.h` only after each wiring stage has passed its bench test.

