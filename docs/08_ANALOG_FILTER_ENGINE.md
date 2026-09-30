# 08 — Analog Filter Engine

## Goal

VORTEX must eventually contain **four physically independent analog low-pass filters**, one for each simultaneous layer.

## Current reference topology

N8 Synth:

`https://www.n8synth.co.uk/diy-eurorack/eurorack-ms-20-lowpass-filter/`

The design is based on the later MS-20-style low-pass concept.

Core characteristics:

- two cascaded OTA stages;
- approximately 12 dB/oct low-pass response;
- LM13700 OTA core;
- nonlinear resonance/feedback behaviour;
- voltage-controlled cutoff;
- Eurorack-oriented ±12 V supply.

## Recommended VORTEX implementation path

Do not build four filters immediately.

Build:

```text
FILTER PROTOTYPE #1
    ↓
test from PCM5102A
    ↓
validate signal level
    ↓
validate cutoff
    ↓
validate resonance
    ↓
validate global cutoff modulation
    ↓
clone ×4
```

## LM13700 requirement

For this topology the LM13700 is required.

None of the currently owned ICs is a direct LM13700 substitute because they are not operational transconductance amplifiers.

Recommended purchase:

- 4 × LM13700 for final filters;
- +1 spare/prototype;
- preferably DIP16 for home soldering.

## Existing ICs and possible VORTEX roles

User currently has:

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

Suggested use:

| IC | VORTEX role |
|---|---|
| LM324 | CV buffers, global cutoff distribution, utility control |
| LM358 | CV/control, LFO, envelope/utility |
| NE5532 | audio buffers, mixer, output stages where suitable |
| NE555 | simple clock/LFO/trigger utilities |
| LM393 | comparators, trigger/sync shaping |
| LM386 | small speaker amplifier only if later required |
| ULN2003 / ULN2803 | LED/relay/load drivers |
| PC817 | isolation / external trigger interface |
| UA741 | generally avoid in main audio path |

## Four-filter signal architecture

```text
DAC OUT L1 -> FILTER 1 ─┐
DAC OUT L2 -> FILTER 2 ─┤
DAC OUT L3 -> FILTER 3 ─┤ -> ANALOG MIXER -> MASTER
DAC OUT L4 -> FILTER 4 ─┘
```

## Global cutoff

Desired physical control:

**GLOBAL CUTOFF**

Concept:

```text
GLOBAL CUTOFF POT
       │
       ▼
CV BUFFER / DISTRIBUTION
       │
   ┌───┼───┬───┐
   ▼   ▼   ▼   ▼
 VCF1 VCF2 VCF3 VCF4
```

LM324 is a good candidate for distributing/buffering the common CV.

The global control should offset all four local cutoff values together rather than replacing their individual settings.

## Global resonance

Desired physical control:

**GLOBAL RESONANCE**

This is not as simple as global cutoff in the N8 topology because resonance is in the audio feedback path.

Options to prototype:

### Option A — simplest Rev.A
Use a 4-gang potentiometer to vary the four feedback paths together.

Pros:
- fully analog;
- simple concept;
- no firmware required.

Cons:
- suitable 4-gang parts are less common;
- tracking tolerances.

### Option B — later voltage-controlled resonance
Insert a VCA/OTA-controlled feedback element in each filter.

Pros:
- global resonance CV;
- automation;
- LFO/envelope control;
- GUI integration.

Cons:
- more components;
- more design work;
- must be prototyped carefully.

No resonance-control implementation is frozen yet.

## Signal-level adaptation

PCM5102A line output is lower than typical Eurorack signal levels.

Therefore the N8 circuit should not simply be copied without checking:

- input attenuation/gain;
- headroom;
- clipping;
- resonance stability;
- output level.

## Power

N8 reference filter expects bipolar analog rails.

VORTEX therefore needs an analog bipolar supply in addition to digital rails.

Candidate overall rails:

```text
+12 V analog
0 V / GND
-12 V analog

+5 V modules/audio
+3.3 V ESP32/logical signals
```

Do not put mains voltage inside the instrument.
