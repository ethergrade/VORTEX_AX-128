# VORTEX AX-128 — Persistent Project Context

This file is the concise starting context for future work in this repository. The detailed specifications linked below remain authoritative; this summary does not replace or silently revise them.

## Canonical working copy and repository

- Canonical local working copy: `/Users/lucasalvatori/Documents/VORTEX_AX-128`
- Canonical GitHub repository: <https://github.com/ethergrade/VORTEX_AX-128>
- Canonical branch: `main`

### Required workflow for every future change

1. Read the current local project files first, including `PROJECT_CONTEXT.md`, `README.md`, and the relevant documents under `docs/`. Check repository status before editing so existing local work is preserved.
2. Treat this local working copy as the source to inspect and edit. Do not assume a cached chat handoff or remote copy is newer than the local files.
3. Before synchronizing, fetch and inspect the canonical `main` branch, compare it with the local branch, and preserve/reconcile any differences. Never overwrite local or remote work without reviewing it.
4. After an appropriate change, update the relevant project documentation and evidence, commit the intended files, and push/synchronize to the canonical repository when the remote is available and the changes are ready.
5. Leave unrelated and untracked user files untouched. If synchronization is unavailable or would conflict, retain the reviewed local change and report the exact state.

## Project state

VORTEX AX-128 is a dedicated four-layer hardware instrument, described as **ANALOG eXTENDED LAYER SYNTHESIZER / ANALOG FILTER • WAVE LAYER**. It is built around an ESP32-S3 Freenove N16R8 (16 MB flash, 8 MB PSRAM, 240 MHz) and an ExpertKeys/Tipro EK-128 keyboard. It is not a modular desktop synth; external synth projects are research references for extracting or independently implementing suitable algorithms.

The current documented milestone is keyboard matrix validation. The EK-128 is reverse engineered as an 8 × 16 diode matrix. The next bench step is the CD74HC4067 scanner on C0/C1 with Row A, followed by all 16 columns and then all eight rows. The scanner firmware is marked `COMPILED_NOT_HARDWARE_TESTED` in `PROJECT_MANIFEST.json`; do not describe the full scanner as hardware validated until the test log confirms it.

The complete EK-128 J1 harness uses 16 even-numbered column pins J1-2 through J1-32 and eight identified odd-numbered row pins J1-3, 5, 7, 9, 11, 13, 15, 17. Remaining odd pins J1-1, 19, 21, 23, 25, 27, 29, 31 are unidentified and must remain disconnected. The initial A1/A2 firmware test only requires C0/J1-2, C1/J1-4, and Row A/J1-3; see `docs/18_EK128_J1_PIN_BY_PIN_WIRING.md` for the complete mapping and staging details.

The architecture target is four simultaneous layers, each routed through its own physically independent analog filter. The prototype DAC target is two PCM5102A stereo DACs providing four mono outputs. See `docs/00_PROJECT_HANDOFF.md` for status and read order, `docs/02_FROZEN_DECISIONS.md` for frozen design choices, and `docs/13_NEXT_STEPS.md` for the active hardware sequence.

## Frozen decisions

`docs/02_FROZEN_DECISIONS.md` is authoritative. Its key frozen points include:

- Freenove ESP32-S3 N16R8 as the main controller.
- Two identical 3.5-inch, 480 × 320, ST7796-class SPI displays.
- Four simultaneous layers and four independent physical analog filters.
- Prototype audio output using two PCM5102A stereo DACs / four mono channels.
- CD74HC4067 keyboard column selection and the documented scanner GPIO assignments.
- Granular as an official VORTEX layer-engine type.
- N8 Synth / later MS-20-inspired analog filter direction, with LM13700 as the intended OTA core.
- Custom KiCad PCB as the final manufacturing direction.

Do not silently change these decisions. A requested revision must be checked against the current documents, then recorded in `docs/02_FROZEN_DECISIONS.md` and the relevant changelog/test log as directed by `docs/00_PROJECT_HANDOFF.md`.

## Open-source DSP research backlog

The research queue is maintained in `docs/15_OPEN_SOURCE_DSP_RESEARCH.md`. It covers Oi, Grandad; Atrix256/GranularSynth; BespokeSynth; FigBug/Wavetable; Terrain; ExclusiveOrange/synthesizer; multichannel-ambient-noise-synthesis; and the Kaelin Bougneit audio pack. The aim is to assess individual DSP ideas for a purpose-built ESP32-S3 four-layer instrument, not to port desktop synth applications wholesale.

Before adapting code, verify the current source, exact license, dependencies, memory and CPU cost, and fit with VORTEX's fixed layer/audio architecture. GPL, unclear-license, framework-dependent, and separately licensed audio assets require the handling documented in the research file. Existing proposed order: native wavetable voice, clean-room granular voice, long algorithmic reverb, procedural ambient scheduler, then a reduced wave-terrain experiment. DSP work follows validation of the keyboard scanner and one stable I2S audio path.

## Long reverb requirement

VORTEX must support very long reverb tails, controlled by physical potentiometers and usable independently on each layer. The target signal path and staged implementation are in `docs/16_LONG_REVERB_ENGINE.md`:

```text
Layer engine → digital long reverb → dedicated DAC channel → that layer's physical analog filter
```

The initial direction is lightweight algorithmic reverb (candidate families include FDN, Dattorro, and Schroeder/Freeverb-style networks), rather than a large convolution engine. Prototype progressively from one instance to two and four; if four independent instances exceed the measured CPU budget, retain per-layer send controls and use one or two shared buses. Physical controls may include per-layer send plus decay, size, damping/color, and optionally pre-delay/modulation. Treat the exact maximum decay times as an experimental target to validate for stability, CPU use, and audio quality.

## Source-of-truth map

- Project overview and current handoff: `README.md`, `docs/00_PROJECT_HANDOFF.md`
- Frozen decisions: `docs/02_FROZEN_DECISIONS.md`
- Hardware, wiring, and scanner: `docs/03_HARDWARE_AND_PINOUT.md`, `docs/04_EK128_MATRIX_REVERSE_ENGINEERING.md`, `docs/18_EK128_J1_PIN_BY_PIN_WIRING.md`, `docs/13_NEXT_STEPS.md`
- DSP research: `docs/15_OPEN_SOURCE_DSP_RESEARCH.md`
- Long reverb: `docs/16_LONG_REVERB_ENGINE.md`
- Test evidence and milestones: `docs/12_TEST_LOG_AND_MILESTONES.md`, `assets/test_evidence/`
