# 13 — Next Steps

## Completed — EK-128 keyboard milestone (reported 2026-10-02)

Active firmware: `firmware/VortexMatrixScanner/`

The scanner is configured for eight rows and sixteen columns. The user confirmed press and release events for all 128 physical keys through the CD74HC4067, exact physical mapping, working multiple presses, and normal mux temperature after correcting reversed VCC/GND. See `docs/12_TEST_LOG_AND_MILESTONES.md` for the test record.

## Next — test the display's integrated microSD slot

The separate six-pin SD breakout failed at CMD0, so try the card reader integrated into the now-working display. The user confirmed `LCD_RST` is already wired to GPIO21; do not connect `SD_CS` there. With power disconnected, connect display `SD_CS` to GPIO47. `VortexSDTest` now defaults to this assignment and holds `LCD_RST` GPIO21 high and `LCD_CS` GPIO1 inactive. Its WAV scan expects `/samples`, so place the test WAV there. Leave the separate SD breakout disconnected. Wiring details are in `docs/20_MICROSD_SPI_TEST_WIRING.md`.

The SD card can be the media/data repository after firmware startup: WAV samples, presets, UI assets, and logs. The normal ESP32-S3 bootloader loads the application from the board's internal SPI flash; the running app then mounts the SD card. See `docs/20_MICROSD_SPI_TEST_WIRING.md` for this distinction.

Historical external-breakout diagnostic: with the display disconnected, Arduino IDE Verbose output showed repeated `sdCommand(): no token received` and `Card Failed! cmd: 0x00` (CMD0), before FAT mounting or WAV discovery. That breakout test used `SD_CS = GPIO21`; this mapping is only for the separate breakout with the display disconnected. It does not apply to the current display-slot test, where `LCD_RST = GPIO21` and `SD_CS = GPIO47`.

Validate:

- FAT filesystem;
- sample directory;
- WAV enumeration;
- PSRAM loading.

## Completed — first SPI display test (user reported 2026-10-02)

The user reports that the SPI display works. Its test sketch is `firmware/VortexDisplayTest/`; the GPIO47 initialization-order fix was compiled and uploaded successfully. For external display power, the pictured Freenove v1.2 breakout accepts 7–12 V DC at its barrel jack and regulates 5V0. Set the matching `VC1` or `VC2` jumper for the display's red supply rail to `5V0` (not `3V3`); red is positive, black is GND. See `docs/21_FIRST_SPI_DISPLAY_WIRING.md` for the power and USB/DC notes. The microSD diagnostic remains paused.

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
