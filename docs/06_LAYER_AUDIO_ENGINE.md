# 06 — Layer and Audio Engine

## Four-layer model

VORTEX is centered around four simultaneously active layers.

A layer is an abstract engine. The firmware should not hard-code a layer to one sound type.

Possible layer types:

- DRUM / SAMPLE
- GRANULAR
- SYNTH
- NOISE
- FX / ONE-SHOT
- future ANALOG source
- future external input

Example scene:

```text
L1 = beat
L2 = granular atmosphere
L3 = pad/synth
L4 = noise/drone
```

Another scene can assign different engine types without changing the core architecture.

## Suggested firmware state

```cpp
struct LayerState {
    bool active;
    bool mute;
    bool solo;

    int engineType;
    int sourceId;

    float level;
    float pan;

    float cutoff;
    float resonance;

    float attack;
    float decay;
    float sustain;
    float release;

    float fxSend;
};
```

Global state should contain all four layers plus transport/sequence/GUI state.

## WAV/sample playback

microSD stores the library.

Short samples should be loaded into PSRAM where practical:

- kick;
- snare;
- hi-hat;
- clap;
- tom;
- percussion.

Long atmospheres/FX can stream from microSD.

## Granular engine

Granular synthesis is an official layer type.

Controls/functions:

- FREEZE
- HOLD/GATE
- RETRIGGER
- REVERSE
- CAPTURE
- RESAMPLE
- SYNC/FREE
- RANDOMIZE
- POSITION
- GRAIN LENGTH
- DENSITY
- PITCH
- ENVELOPE SHAPE
- SPRAY
- CHAOS
- SPACE
- REVERSE %
- SOURCE
- BANK

Initial DSP target:

- 4–8 simultaneous grains;
- increase only after CPU measurements;
- 44.1 or 48 kHz;
- bounded memory use;
- PSRAM sample window/buffer.

Possible initial grain envelope:

- Hann.

## Synth engine

Potential digital oscillator sources:

- saw;
- square;
- triangle;
- sub;
- noise.

Potential signal chain:

```text
OSC1 ─┐
OSC2 ─┤
SUB  ─┤ -> DIGITAL PRE-FILTER -> DAC -> ANALOG FILTER
NOISE ─┘
```

## Digital filter role

A C/C++ MS-20-inspired filter can exist as:

- optional pre-filter;
- automation-friendly filter;
- modulation layer;
- alternate digital mode.

It is **not** intended to replace the four physical analog filters.
