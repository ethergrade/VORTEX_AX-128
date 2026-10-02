# VortexDisplayTest — first SPI LCD smoke test

**Status: COMPILED and uploaded; the user reports that the display works.** The GPIO47 initialization order has been corrected. This is a separate sketch and does not replace the confirmed EK-128 matrix scanner or the paused microSD diagnostic.

Use the full photo-backed wiring and power notes in [`docs/21_FIRST_SPI_DISPLAY_WIRING.md`](../../docs/21_FIRST_SPI_DISPLAY_WIRING.md). The matching display family uses `VCC = 5V`; the separate six-pin microSD breakout continues to use only `3V3`. Do not connect the capacitive-touch I²C pins directly while this display is powered at 5 V.

| Display label | Temporary ESP32-S3 connection |
|---|---|
| `VCC` | `5V` |
| `GND` | `GND` |
| `LCD_CS` | `GPIO1` |
| `LCD_RST` | `GPIO21` |
| `SD_CS` | `GPIO47` (held high during the LCD test) |
| `LCD_RS` / `DC` | `GPIO38` |
| `SDI(MOSI)` | `GPIO14` |
| `SCK` | `GPIO12` |
| `SDO(MISO)` | `GPIO13` |

Leave `LED` and the four `CTP_*` touch pins disconnected. The user's actual wiring has `LCD_RST` on GPIO21 and the display-slot `SD_CS` on GPIO47; the LCD sketch holds the card deselected. Keep the separate SD breakout disconnected.

Open this folder as its own sketch in Arduino IDE. Select `ESP32S3 Dev Module`, 16 MB flash, OPI PSRAM and `USB CDC On Boot: Disabled` for the currently connected USB-to-UART port. Upload, then view Serial Monitor at 115200 baud. The expected visual is red/green/blue vertical bands, a white top edge, a black bottom edge, and a blinking white block near the bottom centre. The user reports that the display works. To power it from the Freenove v1.2 breakout, set the relevant `VC1`/`VC2` red rail to `5V0` and follow the USB/DC caution in `docs/21_FIRST_SPI_DISPLAY_WIRING.md`.

The sketch uses only the installed Arduino ESP32 core (`SPI.h`); it needs no display library. The raw ST7796 initialization follows the [LCDWiki MSP3525/MSP3526 reference](https://www.lcdwiki.com/res/MSP3525_MSP3526/ST7796_Init.txt). The exact module marking and controller are still to be confirmed from the front or successful test.
