# 02 — Frozen Decisions

These decisions are considered frozen unless Luca explicitly requests a revision.

## Product identity

**VORTEX AX-128**  
**ANALOG eXTENDED LAYER SYNTHESIZER**  
**ANALOG FILTER • WAVE LAYER**

## Main controller

Freenove ESP32-S3 development board:

- ESP32-S3-WROOM-1
- N16R8 target
- 16 MB Flash
- 8 MB PSRAM
- 240 MHz
- USB-C programming

## Displays

Two **identical** displays.

Target:

- 3.5"
- IPS
- 480 × 320
- ST7796-class
- SPI
- touch optional, not required for Rev.A

Default logical split:

- left display: selected layer / engine / sequencer;
- right display: filters / FX / mixer / status.

## Layer architecture

Four simultaneous layers are mandatory.

Each layer must eventually pass through its own physically independent analog filter.

Rev.A target:

```text
Layer 1 -> analog filter 1
Layer 2 -> analog filter 2
Layer 3 -> analog filter 3
Layer 4 -> analog filter 4
```

## DAC strategy

Prototype target:

- 2 × PCM5102A stereo DAC
- 4 mono outputs total
- I2S0 -> DAC A -> Layer 1 / Layer 2
- I2S1 -> DAC B -> Layer 3 / Layer 4

## Key matrix scanner

Use CD74HC4067 as 16-channel column selector.

Frozen scanner GPIOs:

```text
S0 = GPIO4
S1 = GPIO6
S2 = GPIO7
S3 = GPIO15
```

Frozen rows:

```text
Row A = GPIO5
Row B = GPIO16
Row C = GPIO17
Row D = GPIO18
Row E = GPIO8
Row F = GPIO9
Row G = GPIO10
Row H = GPIO11
```

## Potentiometer colour language

- RED = DRUM / SAMPLE
- ORANGE = FX
- BLUE = SYNTH
- WHITE = ENVELOPE / FILTER / MODULATION
- BLACK = MASTER / GENERAL

## Granular synthesis

Granular is an official VORTEX layer-engine type.

## Analog filter direction

N8 Synth / later MS-20-inspired low-pass architecture is the current reference topology.

LM13700 is the intended OTA core.

## Final manufacturing

The final design should be produced as a custom KiCad PCB and exported for JLCPCB.

Required final manufacturing files:

- KiCad schematic;
- KiCad PCB;
- Gerber;
- NC drill;
- BOM;
- CPL/position file if assembled;
- board outline;
- production notes.
