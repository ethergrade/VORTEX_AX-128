# 01 — Project Vision

## VORTEX AX-128

**ANALOG eXTENDED LAYER SYNTHESIZER**  
**ANALOG FILTER • WAVE LAYER**

VORTEX AX-128 is a DIY performance instrument built around an ExpertKeys / Tipro EK-128 keyboard matrix and a Freenove ESP32-S3 N16R8.

The design goal is not a simple MIDI controller. VORTEX is intended to be a standalone hybrid instrument containing:

- 128 physical performance/function keys;
- four simultaneous audio layers;
- WAV/sample playback;
- drum machine;
- granular synthesis;
- digital synth/oscillator engines;
- noise/FX/drone engines;
- four physically independent analog filters, one per layer;
- analog master processing;
- two identical graphical displays;
- physical potentiometers;
- 16-step sequencer;
- pattern and scene system;
- stereo line output;
- independent headphone output;
- microSD sample storage;
- USB programming/sample-management path;
- future custom PCB manufactured through JLCPCB.

## Core philosophy

The instrument should feel like dedicated hardware:

- no visible desktop OS;
- immediate boot into VORTEX;
- physical controls first;
- GUI as feedback and navigation;
- minimal menu diving;
- repairable and understandable hardware;
- prototype with modules/millefori first;
- consolidate only validated circuits into the final PCB.

## High-level architecture

```text
EK-128 MATRIX
      │
      ▼
ESP32-S3 N16R8
      │
      ├── Layer Engine
      │     ├── Layer 1
      │     ├── Layer 2
      │     ├── Layer 3
      │     └── Layer 4
      │
      ├── microSD / PSRAM
      ├── Sequencer
      ├── Granular DSP
      ├── Synth DSP
      ├── GUI
      └── USB
            │
            ▼
      2 × stereo DAC
            │
            ├── L1 -> analog filter 1
            ├── L2 -> analog filter 2
            ├── L3 -> analog filter 3
            └── L4 -> analog filter 4
                        │
                        ▼
                 analog mix/master
                        │
             ┌──────────┴──────────┐
             ▼                     ▼
          LINE OUT            HEADPHONE AMP
```

## Construction strategy

Build and validate one subsystem at a time:

1. ESP32-S3 validation.
2. EK-128 matrix reverse engineering.
3. Full keyboard scan.
4. microSD.
5. WAV playback.
6. first DAC.
7. second DAC / four independent layer outputs.
8. layer engine.
9. pots.
10. displays.
11. granular engine.
12. one analog filter prototype.
13. clone analog filter ×4.
14. analog mix/master/FX.
15. full integrated prototype.
16. KiCad schematic.
17. PCB routing.
18. Gerber/BOM/CPL.
19. JLCPCB.
20. enclosure.
