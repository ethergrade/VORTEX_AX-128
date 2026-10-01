# 11 — Component Inventory / BOM Status

## Received / validated

### Controller
- Freenove ESP32-S3 N16R8-class development board
- Freenove ESP32/ESP32-S3 breakout board v1.2

### Keyboard
- ExpertKeys / Tipro EK-128
- key PCB `PCB-1086A`
- original controller `PCB-T2256E`

### Multiplexer
- CD74HC4067 16-channel analog multiplexer module

## Reported on hand; exact board not yet identified

- microSD SPI module and microSD card (user report on 2026-10-02); module photos show a six-pad breakout labeled `3V3`, `CS`, `MOSI`, `CLK`, `MISO`, `GND`; hardware test pending

## Selected / planned

- PCM5102A I2S DAC ×2 target
- TDA1308 headphone amplifier
- 3.5" 480×320 ST7796 display ×2
- B10K linear potentiometers
- assorted potentiometers/trimmers
- LM13700 DIP16 ×5 recommended
- TL072/TL074 may still be purchased for reference filter stages if needed
- resistors/capacitors/diodes/transistors
- 1 kΩ resistors for scanner/protection
- prototyping board / breadboard / headers / connectors

## User-owned IC inventory

```text
NE555
LM324
LM393
UA741
ULN2803
LM358
LM386
NE5532
ULN2003
PC817
```

## Planned roles for existing ICs

- LM324: CV distribution/global cutoff/control
- LM358: control/LFO/envelope utility
- NE5532: audio buffering/mixing/output where suitable
- NE555: clocks/LFO/trigger utility
- LM393: comparator/sync utility
- LM386: optional small speaker amp
- ULN2003/2803: load/LED/relay driving
- PC817: isolation/external trigger
- UA741: avoid in principal audio path

## Not interchangeable

None of the existing ICs is a direct substitute for LM13700 in the chosen MS-20-style OTA filter.
