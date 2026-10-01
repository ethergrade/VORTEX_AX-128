# VortexMatrixScanner

**Status: TESTED — user confirmed all 128 keys, exact mapping, multiple presses, and normal mux temperature on 2026-10-02.**

This scanner reads all 128 coordinates. Each key has a 20 ms debounce window. Do not power the circuit if the multiplexer becomes hot: disconnect USB and diagnose the wiring before running this test.

## Wiring

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
C0..C15          ->  J1-2, J1-4, ... J1-32

ESP32-S3             EK-128 J1
GPIO5, 16, 17, 18 -> J1-3, 5, 7, 9 (Rows A..D)
GPIO8, 9, 10, 11  -> J1-11, 13, 15, 17 (Rows E..H)
```

Keep the original EK-128 controller disconnected. The complete pin mapping and the unidentified pins to leave open are in [`docs/18_EK128_J1_PIN_BY_PIN_WIRING.md`](../../docs/18_EK128_J1_PIN_BY_PIN_WIRING.md).

## Serial Monitor at 115200 baud

```text
VORTEX AX-128 - TEST MATRICE EK-128
Righe: 8 | Colonne: 16 | Antirimbalzo: 20 ms
A1 | LAYER 1 | riga J1-3/GPIO5 | colonna J1-2/C0 | PREMUTO | TESTATI 1/128
A1 | LAYER 1 | riga J1-3/GPIO5 | colonna J1-2/C0 | RILASCIATO
```

Press each physical key and compare its position with the printed coordinate and function name. The count records distinct coordinates seen at least once; it cannot prove that a key is wired to the intended position. Send `m` for missing keys, `p` for the full 128-key map, `r` to reset the count, or `?` for help. After a reset, release and press any keys still held to count them again.

## Compile target

- Board: `ESP32S3 Dev Module`
- FQBN: `esp32:esp32:esp32s3`
- CPU: 240 MHz
- Flash: 16 MB
- PSRAM: OPI 8 MB
- USB CDC On Boot: Enabled

## Bench result

The user confirmed press and release events for every key, exact physical key-to-coordinate correspondence, working multiple presses, and normal mux temperature after correcting an earlier VCC/GND reversal. See [`docs/12_TEST_LOG_AND_MILESTONES.md`](../../docs/12_TEST_LOG_AND_MILESTONES.md). No Serial Monitor capture or exhaustive key-combination matrix was supplied.
