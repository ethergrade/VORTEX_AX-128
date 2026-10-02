# 13 — Next Steps

## Completed — EK-128 keyboard milestone (reported 2026-10-02)

Active firmware: `firmware/VortexMatrixScanner/`

The scanner is configured for eight rows and sixteen columns. The user confirmed press and release events for all 128 physical keys through the CD74HC4067, exact physical mapping, working multiple presses, and normal mux temperature after correcting reversed VCC/GND. See `docs/12_TEST_LOG_AND_MILESTONES.md` for the test record.

## Next — microSD

Prepared firmware: `firmware/VortexSDTest/` (**COMPILED**; serial output confirmed through UART0). The first card attempt reports `microSD non montata`. Use the photo-backed six-pad wiring in `docs/20_MICROSD_SPI_TEST_WIRING.md`; the signal GPIOs are temporary test assignments. The sketch keeps the shared display chip select inactive during the SD test.

Validate:

- FAT filesystem;
- sample directory;
- WAV enumeration;
- PSRAM loading.

## Prepared in parallel — first SPI display

The user supplied the first display's rear pin labels. `firmware/VortexDisplayTest/` is a separate **COMPILED**, hardware-untested LCD smoke test. Use [`21_FIRST_SPI_DISPLAY_WIRING.md`](21_FIRST_SPI_DISPLAY_WIRING.md) to connect only the LCD signals and power; touch and the display's microSD slot remain unused. Confirm the exact front model and photograph the colour-bar result, then record the serial output. This preparation does not change the primary microSD milestone or freeze the final display bus.

## Step 6 — first PCM5102A

Play a WAV through I2S.

## Step 7 — first layer

Key press -> sample engine -> DAC -> audio out.

## Step 8 — second DAC

Establish four independent mono outputs.

## Step 9 — pots

Add second CD74HC4067 or equivalent control multiplexer.

## Step 10 — displays

After the first-display bench result, plan and validate two identical displays together.

## Step 11 — analog filter prototype

Build one N8/MS-20-inspired LM13700 filter on breadboard/millefori.

Do not clone ×4 until validated.
