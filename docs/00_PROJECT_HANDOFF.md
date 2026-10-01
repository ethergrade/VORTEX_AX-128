# VORTEX AX-128 — Project Handoff

## Official identity

**VORTEX AX-128**  
**ANALOG eXTENDED LAYER SYNTHESIZER**  
**ANALOG FILTER • WAVE LAYER**

This repository is the authoritative project baseline. It consolidates the previous chat handoff, photos, screenshots, firmware prototypes and future KiCad work.

## Read order

1. `docs/01_PROJECT_VISION.md`
2. `docs/02_FROZEN_DECISIONS.md`
3. `docs/03_HARDWARE_AND_PINOUT.md`
4. `docs/04_EK128_MATRIX_REVERSE_ENGINEERING.md`
5. `docs/05_KEYMAP_V1_0.md`
6. `docs/06_LAYER_AUDIO_ENGINE.md`
7. `docs/07_GUI_AND_DISPLAYS.md`
8. `docs/08_ANALOG_FILTER_ENGINE.md`
9. `docs/09_POWER_AND_AUDIO_IO.md`
10. `docs/10_PCB_CASE_ROADMAP.md`
11. `docs/11_COMPONENT_INVENTORY_BOM.md`
12. `docs/12_TEST_LOG_AND_MILESTONES.md`
13. `docs/13_NEXT_STEPS.md`
14. `docs/14_OPEN_QUESTIONS.md`
15. `docs/15_OPEN_SOURCE_DSP_RESEARCH.md`
16. `docs/16_LONG_REVERB_ENGINE.md`
17. `docs/17_FIRMWARE_PROJECT_STRUCTURE.md`
18. `docs/18_EK128_J1_PIN_BY_PIN_WIRING.md`
19. `docs/19_ESP32_AUDIO_MIXER_CONTROL_RESEARCH.md`
20. `docs/20_MICROSD_SPI_TEST_WIRING.md`

## Current project status

The Freenove ESP32-S3 N16R8 has been validated on macOS with Arduino IDE:

- CPU: 240 MHz
- Flash: 16 MB
- PSRAM: 8 MB
- USB upload: working
- Serial Monitor: working

The EK-128 matrix has been reverse engineered as an 8 × 16 key matrix with per-key diodes.

A1 and A2 were read successfully by the ESP32-S3 with direct GPIO scanning, including simultaneous/overlapping presses.

On 2026-10-02, the user confirmed that all 128 keys produce press and release events with the CD74HC4067-based scanner, the physical key mapping is exact, multiple presses work, and the mux temperature is normal after correcting reversed VCC/GND wiring. The keyboard milestone is complete; microSD validation is next. See `docs/12_TEST_LOG_AND_MILESTONES.md`.

## Important rule

Do not change frozen GPIO mappings, keymap or architecture silently. Any revision must be explicitly recorded in `docs/02_FROZEN_DECISIONS.md` and the changelog/test log.

## Folder contents

- `docs/` — authoritative project documentation.
- `firmware/` — tested sketches and future scanner skeleton.
- `schematics/` — Mermaid/ASCII architecture references.
- `assets/` — original project photos and screenshots.
- `archive/` — earlier monolithic Markdown documents retained for history.

## Repository workflow

- Keep one prototype outcome per commit.
- Put new test screenshots under `assets/test_evidence/YYYY-MM-DD/`.
- Update the test log and next-step document after each bench test.
- Do not commit phone originals containing GPS metadata; use privacy-safe copies in `assets/`.
