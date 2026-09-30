# 13 — Next Steps

## Immediate Step 1 — CD74HC4067 validation

Wire only:

```text
VCC -> 3.3 V
GND -> common GND
EN -> GND
SIG -> 1 kΩ -> GND

S0 -> GPIO4
S1 -> GPIO6
S2 -> GPIO7
S3 -> GPIO15

C0 -> J1-2
C1 -> J1-4

J1-3 -> GPIO5
```

Test A1/A2 using the mux.

Success condition:

- A1 and A2 remain independently readable;
- overlapping press behaviour remains correct.

## Immediate Step 2 — all 16 columns

Connect:

```text
C0..C15 -> J1 even 2..32
```

Test Row A:

```text
A1..A16
```

## Immediate Step 3 — all 8 rows

Connect:

```text
J1-3  -> GPIO5
J1-5  -> GPIO16
J1-7  -> GPIO17
J1-9  -> GPIO18
J1-11 -> GPIO8
J1-13 -> GPIO9
J1-15 -> GPIO10
J1-17 -> GPIO11
```

Run full 128-key scanner.

## Step 4 — map physical keys to VORTEX keymap

Serial should print both coordinates and function names, for example:

```text
A1 PRESSED - LAYER 1
A10 PRESSED - GRANULAR
C1 PRESSED - BD
G6 PRESSED - DELAY
H16 PRESSED - SCENE 8
```

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
