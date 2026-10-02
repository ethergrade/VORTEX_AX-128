# 13 — Next Steps

## Completed — EK-128 keyboard milestone (reported 2026-10-02)

Active firmware: `firmware/VortexMatrixScanner/`

The scanner is configured for eight rows and sixteen columns. The user confirmed press and release events for all 128 physical keys through the CD74HC4067, exact physical mapping, working multiple presses, and normal mux temperature after correcting reversed VCC/GND. See `docs/12_TEST_LOG_AND_MILESTONES.md` for the test record.

## Next — test the display's integrated microSD slot

The separate six-pin SD breakout failed at CMD0, so try the card reader integrated into the now-working display. Insert the FAT32 card and, with power disconnected, connect the display's printed `SD_CS` pin to GPIO21. The existing `firmware/VortexSDTest/` test already uses GPIO21 for SD chip select and holds `LCD_CS` GPIO1 inactive; its current WAV scan expects `/samples`, so place the test WAV there. Leave the separate SD breakout disconnected. Wiring details are in `docs/20_MICROSD_SPI_TEST_WIRING.md`.

The SD card can be the media/data repository after firmware startup: WAV samples, presets, UI assets, and logs. The normal ESP32-S3 bootloader loads the application from the board's internal SPI flash; the running app then mounts the SD card. See `docs/20_MICROSD_SPI_TEST_WIRING.md` for this distinction.

The user tested with the display disconnected. Arduino IDE Verbose diagnostics now show repeated `sdCommand(): no token received` and `Card Failed! cmd: 0x00` (CMD0). This is a card-response failure during SPI initialization, before FAT mounting or WAV discovery. The core reports the intended pin assignment (SCK=GPIO12, MISO=GPIO13, MOSI=GPIO14, SD_SS=GPIO21); this does not prove physical continuity or power. Next request one clear photo of the replacement module's front/back labels and its wires to the ESP32. Confirm the replacement module's supply requirement from its own silkscreen before applying power; do not assume the old module's pin order or 3.3 V wiring applies.

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
