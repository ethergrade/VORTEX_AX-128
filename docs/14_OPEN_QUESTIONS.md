# 14 — Open Questions / Not Yet Frozen

These points remain open and must not be silently treated as decided.

## Analog filter circuit

- exact N8 schematic adaptation for PCM5102A level;
- exact op-amp choice;
- TL072 versus selected substitutions;
- exact component tolerances;
- filter self-oscillation target;
- calibration requirements.

## Global resonance

Not frozen.

Candidates:

- 4-gang analog potentiometer;
- voltage-controlled feedback/VCA stage;
- OTA-based resonance control.

## Analog power generation

Not frozen.

Need a practical home-buildable way to obtain:

```text
+12 V
-12 V
+5 V
+3.3 V
```

from a safe external DC supply.

## Four-channel analog mixer

Not frozen:

- pan topology;
- level VCAs or passive pots;
- master headroom;
- clipping/drive architecture.

## Display bus arrangement

Two identical ST7796 displays are frozen, but:

- shared SPI bus vs separate SPI peripherals;
- chip-select assignments;
- backlight control;
- optional touch wiring;

are not yet frozen.

## Pot ADC

Second CD74HC4067 address lines may be shared with the keyboard mux, but:

- ADC GPIO;
- sampling timing;
- filtering;
- pot assignment;
- global Cutoff/Resonance analog vs digitally read duplication;

remain to be finalized.

## USB sample-management mode

Concept frozen, implementation not frozen.

Need to choose:

- USB mass storage;
- custom USB app;
- serial file transfer;
- temporary exclusive SD access mode.
