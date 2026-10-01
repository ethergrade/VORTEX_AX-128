# 13 — Next Steps

## Completed — full EK-128 scan (reported 2026-10-02)

Active firmware: `firmware/VortexMatrixScanner/`

The scanner is configured for eight rows and sixteen columns. The user reported press and release events for all 128 physical keys through the CD74HC4067. See `docs/12_TEST_LOG_AND_MILESTONES.md` for the exact scope and the earlier VCC/GND inversion.

## Next check — physical map and electrical stability

1. Confirm the mux stays at a normal temperature with VCC/GND corrected. If it heats again, disconnect power and replace or isolate the damaged module before continuing.
2. Compare each physical key's location and label with the serial coordinate and function name. The 128/128 counter only proves that 128 coordinates generated press events.
3. Test overlapping presses through the mux across different rows and columns, including press/release order.
4. Save one Serial Monitor capture or concise result under `assets/test_evidence/2026-10-02/` if available.

## Step 5 — microSD

Validate:

- FAT filesystem;
- sample directory;
- WAV enumeration;
- PSRAM loading.

## Step 6 — first PCM5102A

Play a WAV through I2S.

## Step 7 — first layer

Key press -> sample engine -> DAC -> audio out.

## Step 8 — second DAC

Establish four independent mono outputs.

## Step 9 — pots

Add second CD74HC4067 or equivalent control multiplexer.

## Step 10 — displays

Bring up one ST7796, then two.

## Step 11 — analog filter prototype

Build one N8/MS-20-inspired LM13700 filter on breadboard/millefori.

Do not clone ×4 until validated.
