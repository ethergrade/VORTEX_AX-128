# 03 — Hardware and Frozen Pinout

For one complete 32-pin wiring table, use [`18_EK128_J1_PIN_BY_PIN_WIRING.md`](18_EK128_J1_PIN_BY_PIN_WIRING.md).

For the separate microSD and first-display bench wiring, use [`20_MICROSD_SPI_TEST_WIRING.md`](20_MICROSD_SPI_TEST_WIRING.md) and [`21_FIRST_SPI_DISPLAY_WIRING.md`](21_FIRST_SPI_DISPLAY_WIRING.md). Their SPI GPIOs are temporary and do not revise the frozen keyboard scanner map.

## 1. EK-128 hardware

Observed board identifiers:

- key PCB: `PCB-1086A`
- original controller: `PCB-T2256E`
- key/controller connector: `J1`, 2 × 16 / 32 pins
- original USB: `J2`

The original controller must stay disconnected during ESP32 matrix testing.

## 2. Freenove ESP32-S3

Received board:

- ESP32-S3-WROOM-1
- marking consistent with N16R8
- 16 MB Flash
- 8 MB PSRAM

Board orientation used in this project:

```text
ANTENNA
  ↑

LEFT EDGE              RIGHT EDGE
3V3                    TX
EN                     RX
4                      1
5                      2
6                      42
7                      41
15                     40
16                     39
17                     38
18                     37
8                      36
3                      35
46                     0
9                      45
10                     48
11                     47
12                     21
13                     20
14                     19
5V                     GND

        USB-C USB-C
```

Use the actual Freenove silkscreen/photos as the physical authority.

## 3. Keyboard CD74HC4067 wiring

### ESP32 -> CD74HC4067

| ESP32 | CD74HC4067 |
|---|---|
| 3V3 | VCC |
| GND | GND |
| GND | EN |
| GPIO4 | S0 |
| GPIO6 | S1 |
| GPIO7 | S2 |
| GPIO15 | S3 |
| GND through ~1 kΩ | SIG |

`EN` is active-low, therefore tying it to GND keeps the mux enabled.

`SIG -> 1 kΩ -> GND` means the selected mux channel is pulled LOW.

## 4. CD74HC4067 -> EK-128 columns

| MUX | EK-128 | Physical column |
|---|---:|---:|
| C0 | J1-2 | 1 |
| C1 | J1-4 | 2 |
| C2 | J1-6 | 3 |
| C3 | J1-8 | 4 |
| C4 | J1-10 | 5 |
| C5 | J1-12 | 6 |
| C6 | J1-14 | 7 |
| C7 | J1-16 | 8 |
| C8 | J1-18 | 9 |
| C9 | J1-20 | 10 |
| C10 | J1-22 | 11 |
| C11 | J1-24 | 12 |
| C12 | J1-26 | 13 |
| C13 | J1-28 | 14 |
| C14 | J1-30 | 15 |
| C15 | J1-32 | 16 |

## 5. EK-128 rows -> ESP32

| Row | EK-128 J1 | ESP32 |
|---|---:|---:|
| A | J1-3 | GPIO5 |
| B | J1-5 | GPIO16 |
| C | J1-7 | GPIO17 |
| D | J1-9 | GPIO18 |
| E | J1-11 | GPIO8 |
| F | J1-13 | GPIO9 |
| G | J1-15 | GPIO10 |
| H | J1-17 | GPIO11 |

Firmware mode: all row GPIOs are `INPUT_PULLUP`.

## 6. Unused / unknown EK-128 J1 pins

Leave disconnected until identified:

```text
J1-1
J1-19
J1-21
J1-23
J1-25
J1-27
J1-29
J1-31
```

## 7. ESP32 pins reserved / avoided

Reserved:

- GPIO2 -> onboard status/debug LED
- GPIO19 / GPIO20 -> native USB
- TX/RX -> serial/programming diagnostics

Avoid for normal Rev.A project I/O:

- GPIO0
- GPIO3
- GPIO45
- GPIO46
- GPIO35
- GPIO36
- GPIO37

## 8. Future pot multiplexer

A second CD74HC4067 for potentiometers can share:

```text
S0 = GPIO4
S1 = GPIO6
S2 = GPIO7
S3 = GPIO15
```

Only its `SIG` requires a separate ADC input.

Keyboard scanning and pot acquisition should occur in separate firmware time slots.
