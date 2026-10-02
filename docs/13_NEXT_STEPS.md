# 13 — Next Steps

## Completed — EK-128 keyboard milestone (reported 2026-10-02)

Active firmware: `firmware/VortexMatrixScanner/`

The scanner is configured for eight rows and sixteen columns. The user confirmed press and release events for all 128 physical keys through the CD74HC4067, exact physical mapping, working multiple presses, and normal mux temperature after correcting reversed VCC/GND. See `docs/12_TEST_LOG_AND_MILESTONES.md` for the test record.

## Paused — microSD

Prepared firmware: `firmware/VortexSDTest/` (**COMPILED**; serial output confirmed through UART0). The card still reports `microSD non montata` after the user soldered a replacement SD module. The card is FAT32, named `VAULT`, with the WAV currently in its root. The sketch's `/samples` requirement applies only after mounting succeeds, so it does not explain this mount error. Use the photo-backed six-pad wiring in `docs/20_MICROSD_SPI_TEST_WIRING.md`; the signal GPIOs are temporary test assignments.

The user tested with the display disconnected. Arduino IDE Verbose diagnostics now show repeated `sdCommand(): no token received` and `Card Failed! cmd: 0x00` (CMD0). This is a card-response failure during SPI initialization, before FAT mounting or WAV discovery. The core reports the intended pin assignment (SCK=GPIO12, MISO=GPIO13, MOSI=GPIO14, SD_SS=GPIO21); this does not prove physical continuity or power. Next request one clear photo of the replacement module's front/back labels and its wires to the ESP32. Confirm the replacement module's supply requirement from its own silkscreen before applying power; do not assume the old module's pin order or 3.3 V wiring applies.

Validate:

- FAT filesystem;
- sample directory;
- WAV enumeration;
- PSRAM loading.

## Next — first SPI display test

Prepared firmware: `firmware/VortexDisplayTest/` (**COMPILED AND UPLOADED**). The sketchbook copy compiled at 338,580 program bytes / 22,824 global bytes and uploaded with flash verification. The user reports no visible image. The earlier GPIO47 initialization warning is fixed; the current Verbose GPIO map recognizes GPIO47. Before changing SPI wiring or controller code, identify whether the backlight glows, verify VCC/GND against the printed labels with power disconnected, and obtain a clear photo of the actual display-to-ESP32 wiring. Follow `docs/21_FIRST_SPI_DISPLAY_WIRING.md` only for the pictured matching module family: display VCC=5V, GND, `LCD_CS=GPIO1`, `LCD_RST=GPIO47`, `LCD_RS=GPIO38`, `SDI=GPIO14`, `SCK=GPIO12`, `SDO=GPIO13`; leave display `SD_CS`, `LED`, and all touch pins open. Keep the separate SD breakout disconnected for this isolated test. Expected result is RGB bands with a blinking white square. Resume microSD after recording a successful display result or a clear diagnosis.

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
