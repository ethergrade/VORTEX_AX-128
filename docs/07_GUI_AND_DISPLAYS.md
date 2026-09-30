# 07 — GUI and Displays

## Display hardware

Two identical displays:

- 3.5"
- IPS
- 480 × 320
- ST7796-class
- SPI

Touch is optional and not needed for the first version.

## Default roles

### Left display

Main contextual display:

- selected layer;
- selected engine;
- sample name;
- granular state;
- synth parameters;
- sequencer;
- pattern;
- active keys.

### Right display

Persistent technical display:

- Filter 1–4 state;
- global cutoff/resonance;
- FX;
- mixer levels;
- CPU;
- PSRAM;
- transport/status.

Both displays may change pages.

## Navigation

Physical EK-128 keys select GUI pages:

```text
A9  DRUM
A10 GRANULAR
A11 SYNTH
A12 ANALOG
A13 FX
A14 SEQ
A15 MIX
A16 SYSTEM
```

Layer keys:

```text
A1 LAYER 1
A2 LAYER 2
A3 LAYER 3
A4 LAYER 4
```

## Visual style

Target aesthetic:

- retro workstation;
- sampler/synth 1980s/1990s;
- CRT-inspired;
- pixel/technical typography;
- segmented bars;
- small VU meters;
- scanline-like visual treatment;
- dense but readable status information.

Possible themes:

- CRT GREEN
- AMBER
- ICE BLUE

## GUI architecture

No visible desktop OS.

Recommended software concept:

- ESP-IDF / Arduino core underlying runtime;
- FreeRTOS internally;
- LVGL or similarly lightweight GUI layer;
- user sees only VORTEX.

GUI must not own audio state.

Use a central `GrooveboxState` / `VortexState` shared by:

- keyboard;
- pots;
- sequencer;
- audio;
- GUI.

Display rendering must remain lower priority than audio.

Avoid full-screen redraws when not required.

## Example layer page

```text
┌────────────────────────────────────────┐
│ VORTEX AX-128              BPM 112     │
├────────────────────────────────────────┤
│ L1 BEAT      ████████ ACTIVE           │
│ L2 GRANULAR  █████    ACTIVE           │
│ L3 SYNTH     ███████  ACTIVE           │
│ L4 NOISE     ██       MUTE             │
├────────────────────────────────────────┤
│ SELECTED: LAYER 2                      │
│ POSITION 43%       GRAINS 18/s         │
│ LENGTH 124 ms      TUNE -7 st          │
│ CHAOS 31%          SPACE 72%           │
│ FILTER 3.8 kHz     RESO 48%            │
└────────────────────────────────────────┘
```
