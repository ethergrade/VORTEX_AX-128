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

The user confirmed press and release events for all 128 keys through the CD74HC4067 on 2026-10-02, exact physical mapping, working multiple presses, and normal mux temperature after correcting reversed VCC/GND. See `docs/12_TEST_LOG_AND_MILESTONES.md`.

## Next diagnostic sketch

`VortexSDTest/` is a separate **COMPILED** microSD SPI/WAV/PSRAM test. The user's photographed breakout is labeled `3V3` and has six identified pads. See its README and `docs/20_MICROSD_SPI_TEST_WIRING.md` for the temporary wiring and generated test WAV.

`VortexDisplayTest/` is a separate **COMPILED** first-LCD colour-bar test for the photographed 14-pin ST7796-family module. It shares the temporary SPI clock/data GPIOs with `VortexSDTest`, uses its own LCD chip-select, and does not use touch or the display's onboard SD slot. See its README and `docs/21_FIRST_SPI_DISPLAY_WIRING.md`. Physical display operation is still **UNTESTED**.
