# 15 — Open-Source DSP Research Backlog

This document is the research queue for later analysis in ChatGPT Desktop / Work.

The goal is **not** to turn VORTEX into a modular desktop synth. VORTEX remains a dedicated four-layer hardware instrument on ESP32-S3. External projects are references for algorithms, architecture and sound design, and any code reuse must first pass technical and license review.

## Source-level review — 2026-09-30

| Candidate | License/dependency reality | Embedded value | Decision |
|---|---|---|---|
| Oi, Grandad! | GPL-3.0, HISE/JUCE, large generated desktop project | Parameter model, playheads, drone/freeze, per-voice delay | UX/behaviour reference only |
| Atrix256 GranularSynth | No explicit license; single offline Windows C++ program | Grain windows, overlap and interpolation as teaching material | Reference only; do not copy |
| BespokeSynth | GPL-3.0 desktop modular host | Routing/modulation workflow | Architecture inspiration only |
| FigBug Wavetable | BSD-3-Clause repository, but oscillator is supplied by Gin/JUCE and wavetable assets have separate terms | Voice layout, table sizes, unison/position parameter model | Reimplement a small native oscillator |
| Terrain | GPL-3.0, JUCE/MTS-ESP, expensive per-sample terrain and trajectory math | Novel reduced wave-terrain engine | Experimental mono prototype only |
| ExclusiveOrange synthesizer | No explicit license | Procedural/stochastic event and VM concepts | Conceptual reference only |
| Multichannel ambient synthesis | MIT Python/STFT research code | Offline texture generation and spatial ideas | Offline tool/reference, not firmware |
| Kaelin Bougneit pack | CC BY-SA 4.0 audio content | Dark ambient aesthetic | Do not import into firmware/repository |

### Important code observations

- `Atrix256/GranularSynth/Source.cpp` is an offline file processor, not a real-time voice engine. It allocates complete input/output vectors and writes WAV files. Its useful concepts are cubic Hermite sampling, grain overlap and crossfade windows. The disabled linear-interpolation branch appears to read the first sample twice, which reinforces that it should not be treated as production firmware.
- `FigBug/Wavetable/plugin/Source/WavetableVoice.*` delegates the actual oscillator to `gin::WTOscillator`. The repository is useful for the surrounding voice design, but the embedded oscillator still has to be designed independently. The documented table lengths of 256, 512, 1024 and 2048 samples fit an ESP32-S3 implementation well.
- `Terrain/Source/DSP/Trajectory.h` calculates trajectory and terrain at audio rate and allocates a two-second point-feedback buffer per voice. VORTEX should omit feedback and oversampling in the first experiment, precompute terrain data, and start monophonic.
- Oi, Grandad! exposes four granular voices, up to four playheads per voice and per-voice delay, but the implementation is tied to HISE-generated networks. It is a strong control/feature reference and a poor porting target.

## VORTEX implementation order

The recommended order after the matrix and DAC milestones is:

1. **Native wavetable oscillator** — one table, one voice, linear interpolation, then multiple table positions.
2. **Clean-room granular voice** — mono sample in PSRAM, 4–8 grains, deterministic scheduler, simple Hann window.
3. **Long algorithmic reverb** — first one bus, then compare two buses and four independent instances.
4. **Procedural ambient scheduler** — stochastic triggers and parameter drift using native VORTEX engines.
5. **Reduced wave-terrain experiment** — monophonic, precomputed terrain, no feedback, strict CPU gate.

None of these starts until the keyboard scanner and one stable I2S audio path are validated. This prevents DSP research from hiding basic hardware timing problems.

## Repositories to explore

### publicsamples/Oi-Grandad
GitHub: https://github.com/publicsamples/Oi-Grandad

Current relevance:
- 4-voice granular synthesizer
- extensive per-voice granular controls
- complex modulation sources
- drone/sustained playback concept
- delay per voice
- newer granular engine in V2
- HISE-based

License observed: GPL-3.0.

VORTEX research target:
- granular voice lifecycle
- multi-playhead ideas
- grain scheduling
- modulation model
- drone/freeze behavior
- per-voice FX concepts

Do not directly transplant HISE/desktop architecture into VORTEX.

### Atrix256/GranularSynth
GitHub: https://github.com/Atrix256/GranularSynth

Current relevance:
- compact C++ granular synthesis reference
- associated Demofox article on granular audio synthesis
- potentially useful as a minimal algorithmic teaching reference

License status:
- no license file found during initial review

Rule:
Treat as study/reference only unless permission/license is clarified.

### BespokeSynth/BespokeSynth
GitHub: https://github.com/BespokeSynth/BespokeSynth

Current relevance:
- mature modular software synth
- live patching
- modulation/routing
- effects architecture
- controller mapping

License observed: GPL-3.0.

VORTEX research target:
- conceptual routing
- modulation graph ideas
- sequencer/FX interaction
- UI/workflow inspiration

Not a direct port target.

### FigBug/Wavetable
GitHub: https://github.com/FigBug/Wavetable

Current relevance:
- 2 wavetable oscillators
- custom WAV wavetable loading
- unison
- sub oscillator
- noise generator
- modulation matrix
- delay
- reverb
- step sequencing

README states BSD licensing, but the project depends on JUCE; commercial JUCE licensing implications must be separated from any reusable DSP code.

VORTEX research target:
- wavetable interpolation
- table format/loading
- oscillator phase handling
- anti-alias strategy
- low-memory wavetable engine
- modulation design
- simple embedded-compatible reverb/delay ideas

### aaronaanderson/Terrain
GitHub: https://github.com/aaronaanderson/Terrain

Current relevance:
- wave-terrain synthesis
- 2D trajectory scanning a 3D surface
- time-varying timbre
- trajectory feedback
- meanderance
- oversampling for alias reduction

License observed: GPL-3.0.

Important technical warning:
The README explicitly describes Terrain as computationally expensive because trajectory and terrain can be calculated per sample, and oversampling further increases cost.

VORTEX research target:
Create a **reduced embedded Wave/Terrain engine**, not a direct port:
- precomputed terrain tables
- simplified trajectory families
- lower update rate for modulation
- bounded oversampling
- optional low-poly / low-resolution terrain mode
- benchmark CPU before adding to the 4-layer engine

### ExclusiveOrange/synthesizer
GitHub: https://github.com/ExclusiveOrange/synthesizer

Current relevance:
- procedural sound-description virtual machine
- stochastic synthesis
- many concurrent lightweight sound programs
- ambient generation examples: rain, water, wind chimes
- hardware-agnostic sound descriptions

License status:
- no license file found during initial review

Rule:
Study concepts only unless reuse permission/license is clarified.

VORTEX research target:
- procedural ambient layer
- stochastic event generation
- low-memory ambient textures
- generative layer presets

### suwoncjh/multichannel-ambient-noise-synthesis
GitHub: https://github.com/suwoncjh/multichannel-ambient-noise-synthesis

Current relevance:
- Python research implementation
- STFT spatial covariance/coherence synthesis
- time-varying ambient fields
- RIR-based reverberation path

License observed: MIT.

Technical fit:
Not a direct real-time ESP32 port target. STFT/SCM multichannel processing is much heavier than VORTEX requires.

VORTEX research target:
- ambient/noise design concepts
- offline generation of source textures
- room/reverb concepts
- ideas for spatial coloration simplified to stereo

### onura46/Kaelin-Bougneit-CDDA-Music-Pack
GitHub: https://github.com/onura46/Kaelin-Bougneit-CDDA-Music-Pack

This is primarily **audio content**, not a DSP engine.

Relevance:
- dark ambient aesthetic reference
- beatless texture reference
- possible test/reference material only if licensing is respected

License observed: CC BY-SA 4.0.

If audio is ever bundled, adapted or distributed with VORTEX, attribution/share-alike obligations must be reviewed.

---

## License policy for VORTEX

Before adapting any external project, classify it:

### Permissive
MIT/BSD-style:
- potentially reusable after checking dependencies and notices
- still document attribution/license obligations

### GPL
GPL projects can be studied freely, but directly integrating GPL code into distributed VORTEX firmware can impose GPL obligations on the combined firmware.

For a potentially distributable/commercial VORTEX:
- prefer clean-room reimplementation from documented algorithms/papers
- or use permissively licensed DSP code

### No explicit license
Public source code is **not automatically reusable**.

Use only as a behavioral/conceptual reference until explicit permission or license is identified.

### Framework-dependent
A permissively licensed repository may still depend on JUCE/HISE or another framework with separate licensing constraints.

Always separate:
1. DSP algorithm license
2. framework license
3. assets/wavetables/sample licenses

---

## Desktop research method

For each candidate repository:

1. identify exact source files implementing the DSP engine;
2. identify external dependencies;
3. identify license of repository and dependencies;
4. isolate the algorithm from GUI/plugin framework;
5. estimate RAM/PSRAM usage;
6. estimate operations per sample / per block;
7. map parameters to VORTEX layer controls;
8. create a minimal ESP32-S3 C/C++ prototype;
9. benchmark CPU at 48 kHz;
10. benchmark with granular + GUI + matrix scan active;
11. keep only algorithms that remain deterministic and glitch-free.

No repo should be considered “integrated” until those tests pass.
