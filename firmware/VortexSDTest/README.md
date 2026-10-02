# VortexSDTest — microSD, WAV, PSRAM

**Status: COMPILED; not yet tested with a physical microSD module.** This is a separate Arduino sketch; it does not replace the working `VortexMatrixScanner` keyboard firmware.

## Before connecting the module

The user's module photos show a six-pad SPI breakout with a rear `3V3` supply label. Use **ESP32 `3V3`**, never `5V`, for this board. With the rear facing you, socket left and pads right, the labels are top-to-bottom: `3V3`, `CS`, `MOSI`, `CLK`, `MISO`, `GND`. Check the actual rear labels before wiring. The full table is in [`docs/20_MICROSD_SPI_TEST_WIRING.md`](../../docs/20_MICROSD_SPI_TEST_WIRING.md).

The sketch proposes these **temporary test signal pins** on the Freenove ESP32-S3; they do not overlap the keyboard scanner pins and are not frozen for the final PCB:

| Module pad | ESP32-S3 test connection |
|---|---|
| 3V3 | 3V3 |
| CS | GPIO21 |
| MOSI | GPIO14 |
| CLK | GPIO12 |
| MISO | GPIO13 |
| GND | GND |

Change the four signal-pin constants at the top of `VortexSDTest.ino` if the temporary pin choice must change.

## Prepare the card

Use a card with a FAT filesystem. On the computer, create `/samples` and copy [`VORTEX_TEST_440HZ.wav`](../../assets/test_samples/VORTEX_TEST_440HZ.wav) into it. The file is a generated 1-second, 44.1 kHz, mono, 16-bit PCM test tone. Keep existing card data; the sketch only reads and does not format or write to the card.

## Run

1. Open `VortexSDTest.ino` in Arduino IDE. Select `ESP32S3 Dev Module`, 16 MB flash, OPI 8 MB PSRAM, and `USB CDC On Boot: Enabled`.
2. With USB disconnected, make the six connections exactly as labeled on the module, check them, then upload this separate test sketch.
3. Open Serial Monitor at 115200 baud and reset the ESP32-S3 if the initial messages were missed.

The sketch mounts the card at 4 MHz SPI, reports capacity, lists `.wav` files in `/samples`, checks RIFF/WAVE PCM16 mono/stereo headers, and copies up to 64 KiB of the first compatible file's audio data to PSRAM. A checksum is printed to show the bytes were read. The scan is read-only and does not play audio.

Expected successful lines include:

```text
Scheda rilevata. Capacita: ... MB. PSRAM libera: ... KB
WAV 1: VORTEX_TEST_440HZ.wav (88244 byte)
  PCM16: 1 canale/i, 44100 Hz, 88200 byte audio
PSRAM OK: 65536 byte WAV copiati, checksum FNV-1a 49EA2D8F
Risultato: 1 WAV, 1 PCM16 compatibili, PSRAM OK.
```

`49EA2D8F` is the expected checksum for the first 65,536 audio bytes of the supplied test WAV. A different WAV will have a different checksum.

The software is ready to compile; card detection and the physical SPI wiring still require a bench test.

## No text in Serial Monitor

The sketch starts the one-shot diagnostic 1.5 seconds after reset and does not wait on the CDC connection-state flag. Open Serial Monitor on the current `/dev/cu.usbmodem...` port at **115200 baud**, then press the ESP32-S3 `RST` button once. If the startup lines were missed, reset once more with the monitor open. The test prints card/WAV/PSRAM results as text; it does not play audio, so silence at the audio output is expected.

This diagnostic reads the WAV and copies up to 64 KiB into PSRAM. **It does not play audio**, so silence at the audio output is expected during this test. Successful results appear as text in Serial Monitor.

## Saved board configuration

The checked-in [`sketch.yaml`](sketch.yaml) stores the complete default board identifier and menu options for Arduino CLI. With Arduino CLI, run this from the sketch folder to use the settings without typing them again:

```sh
arduino-cli compile .
arduino-cli upload . --port /dev/cu.usbmodemXXXXXXXXXXXX
```

For Arduino IDE, select these values in the board/tool menus:

| IDE setting | Value |
|---|---|
| Board | `ESP32S3 Dev Module` |
| USB Mode | `Hardware CDC and JTAG` |
| USB CDC On Boot | `Enabled` |
| CPU Frequency | `240MHz (WiFi)` |
| Flash Mode | `QIO 80MHz` |
| Flash Size | `16MB` |
| Partition Scheme | `16M Flash (3MB APP / 9.9MB FATFS)` |
| PSRAM | `OPI PSRAM` |
| Upload Speed | `921600` |

Arduino IDE does not show this file as a custom profile menu; select the same values in its menus. The full board selection has also been recorded below so it stays with the project on GitHub. Use ESP32 core **3.3.12**.
