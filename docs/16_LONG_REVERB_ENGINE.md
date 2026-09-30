# 16 — Long Reverb Engine

## Requirement

VORTEX should support **very long reverbs**, controllable from physical potentiometers and usable on individual layers.

The default goal is not convolution reverb. The first implementation should use a lightweight **algorithmic reverb**.

## Why algorithmic first

A 20–60 second audible decay does not require storing 20–60 seconds of audio.

A feedback network can create a very long RT60 using relatively short delay lines.

Good candidate families:

- Schroeder
- Freeverb-style comb/allpass
- Feedback Delay Network (FDN)
- Dattorro plate-style topology

These are much more realistic for ESP32-S3 than a long stereo convolution IR.

## Position in the VORTEX signal chain

Preferred per-layer digital placement:

```text
LAYER ENGINE
    │
    ├── digital EQ/pre-filter
    │
    ├── LONG REVERB
    │
    ▼
DEDICATED DAC CHANNEL
    │
    ▼
PHYSICAL ANALOG FILTER
```

This preserves the mandatory physically independent analog filter after the digital layer processing.

## Independent reverb target

Ideal target:

```text
Layer 1 -> Reverb 1 -> DAC -> Filter 1
Layer 2 -> Reverb 2 -> DAC -> Filter 2
Layer 3 -> Reverb 3 -> DAC -> Filter 3
Layer 4 -> Reverb 4 -> DAC -> Filter 4
```

Prototype progressively:

1. one reverb instance;
2. two instances;
3. four instances;
4. benchmark together with granular synthesis and displays.

If four independent engines exceed the CPU budget, fallback:

```text
L1 ─send─┐
L2 ─send─┤
L3 ─send─┤ -> one or two shared long-reverb buses
L4 ─send─┘
```

Each layer still retains an independent physical **REVERB SEND** amount.

## Physical controls

Desired contextual controls for selected layer:

- REVERB SEND
- DECAY
- SIZE
- DAMP / COLOR
- optional PRE-DELAY
- optional MODULATION

A dedicated physical pot may always represent `REVERB SEND` for the selected layer.

Alternative panel idea:

```text
L1 REV SEND
L2 REV SEND
L3 REV SEND
L4 REV SEND
GLOBAL DECAY
GLOBAL DAMP
```

This is mechanically simple and very immediate.

## Very long tail

Target modes could be:

- ROOM: ~0.5–3 s
- HALL: ~2–12 s
- DEEP: ~8–30 s
- INFINITE: feedback/freeze-like mode

The exact numeric RT60 limits will be decided after stability and CPU tests.

## Memory strategy

Delay buffers can live in PSRAM where latency/performance is acceptable.

Avoid allocating and freeing buffers during playback.

Allocate reverb memory at boot or when switching engine mode.

Possible sample representation:
- float internally for early prototype;
- later benchmark float vs fixed-point / int16 delay storage.

At 48 kHz, raw mono delay storage costs approximately:

| Total delay time | `float` | `int16_t` |
|---:|---:|---:|
| 0.5 s | 96 KB | 48 KB |
| 1.0 s | 192 KB | 96 KB |
| 4.0 s | 768 KB | 384 KB |

The audible decay time can be far longer than the physical delay storage because RT60 is produced by feedback. A first FDN should target roughly 0.3–1.0 seconds of total delay storage per instance, then tune the feedback for long tails.

## CPU strategy

- process audio in blocks;
- no `delay()` calls in DSP;
- precompute coefficients when controls change;
- avoid expensive transcendental math per sample;
- smooth potentiometer changes;
- update UI at a much lower rate than audio.

## Acceptance gates

Each step must run at 48 kHz without audio underruns:

1. dry audio + one reverb instance;
2. granular test voice + one reverb instance;
3. four dry layers + one shared reverb bus;
4. four layers + two shared buses;
5. only then test four independent instances.

Record CPU load, free internal RAM, free PSRAM and the longest stable decay. If four instances fail, retain four independent send controls and use one or two shared buses.

## Granular + reverb

A particularly important VORTEX combination:

```text
GRANULAR ATMOSPHERE
     │
     ▼
LONG REVERB
     │
     ▼
DEDICATED MS-20-STYLE ANALOG FILTER
```

This should be treated as one of the main benchmark scenarios.

## Reverb research targets

When reviewing the listed open-source repositories, specifically search for:
- delay-line structures;
- damping filters;
- diffusion/allpass stages;
- feedback matrices;
- freeze/infinite-tail behavior;
- modulation of delay lengths;
- denormal handling;
- coefficient smoothing.

Prefer permissively licensed standalone DSP over plugin-framework-dependent code.
