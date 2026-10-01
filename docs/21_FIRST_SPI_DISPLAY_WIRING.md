# 21 — First SPI display wiring and smoke test

## Identification and scope

The user's rear photo [`IMG_8864_no_metadata.jpg`](../assets/Displays/IMG_8864_no_metadata.jpg) shows a 14-pin SPI TFT module with capacitive-touch flex, an onboard microSD socket, a 74LVC245A buffer and a small onboard regulator. Its labels and component layout match the LCDWiki MSP3526 / Elecrow DLS30035B family, which uses an ST7796-class 320 × 480 panel. **The exact model and front-side size marking have not yet been independently confirmed.** The first test is therefore a cautious LCD-only smoke test for this identified board family; the final two-display arrangement remains open.

The manufacturer's [module page](https://www.lcdwiki.com/3.5inch_IPS_SPI_Module_ST7796) specifies a 5 V working supply and approximately 0.5 W for the matching 3.5-inch module. Its [schematic](https://www.lcdwiki.com/res/MSP3525_MSP3526/3.5inch_SPI_Module_MSP3525_MSP3526_Schematic.pdf) shows the regulator, SPI buffers and touch I²C level-shifting circuit. The manufacturer also states that the `LED` pin may be left open for an enabled backlight. The rear silkscreen on the actual board remains the physical authority for pin order.

**Disconnect USB power before wiring.** Check the display's exact product label or listing before applying power if it differs from this board family. The display `VCC` goes to the Freenove **`5V`** rail for this matching module; the separate six-pin microSD breakout is marked **`3V3`** and must still go only to **`3V3`**. Both share GND. Never exchange these two power rails.

## Complete 14-pin rear-header map

With the **rear as photographed**, the header is at left; the top pin is `SD_CS` and the bottom pin is `VCC`. Number 1 is therefore at the **bottom**. Use the printed labels if the board is turned around.

| Rear top → bottom | Header pin | Display label | Freenove ESP32-S3, first test | Meaning |
|---:|---:|---|---|---|
| 1 | 14 | `SD_CS` | Leave open | Selects the display's *own* microSD slot; unused |
| 2 | 13 | `CTP_INT` | Leave open | Capacitive-touch interrupt; unused |
| 3 | 12 | `CTP_SDA` | Leave open | Capacitive-touch I²C data; unused |
| 4 | 11 | `CTP_RST` | Leave open | Capacitive-touch reset; unused |
| 5 | 10 | `CTP_SCL` | Leave open | Capacitive-touch I²C clock; unused |
| 6 | 9 | `SDO(MISO)` | `GPIO13` | Shared SPI data back to ESP32 |
| 7 | 8 | `LED` | Leave open | Backlight defaults on for matching module |
| 8 | 7 | `SCK` | `GPIO12` | Shared SPI clock |
| 9 | 6 | `SDI(MOSI)` | `GPIO14` | Shared SPI data from ESP32 |
| 10 | 5 | `LCD_RS` | `GPIO38` | Command/data selection (`DC`) |
| 11 | 4 | `LCD_RST` | `GPIO47` | Display reset |
| 12 | 3 | `LCD_CS` | `GPIO1` | Display chip select, active low |
| 13 | 2 | `GND` | `GND` | Common ground |
| 14 | 1 | `VCC` | `5V` | Display-module power for matching 5 V board |

These are **temporary bench GPIOs**. `GPIO12`/`13`/`14` deliberately match the external microSD diagnostic. That breakout keeps its separate `CS = GPIO21`; the display uses `LCD_CS = GPIO1`. [`VortexDisplayTest.ino`](../firmware/VortexDisplayTest/VortexDisplayTest.ino) sets GPIO21 high so an already-connected external microSD breakout remains inactive. Do not use the display's onboard card slot in this test, and do not connect its `SD_CS` to GPIO21.

With display `VCC = 5V`, the matching schematic pulls the external touch I²C lines up to `VCC`. **Do not attach `CTP_SCL` or `CTP_SDA` directly to 3.3 V ESP32 GPIOs in this configuration.** Touch needs a separately checked level-safe wiring plan. Leave all four `CTP_*` pins open now. The matching schematic says `LED` open leaves the backlight on; an unlit panel alone does not prove SPI is working.

## First test

1. Leave the working EK-128/multiplexer wiring alone. With USB unplugged, connect the eight display wires shown above: `VCC`, `GND`, `LCD_CS`, `LCD_RST`, `LCD_RS`, `SDI`, `SCK`, and `SDO`. `LED`, touch and display-slot `SD_CS` remain open.
2. Recheck the *physical* top and bottom labels before powering the board; ensure `5V` goes only to display `VCC` and `3V3` only to the separate microSD module.
3. In Arduino IDE open `VortexDisplayTest/VortexDisplayTest.ino`. Select `ESP32S3 Dev Module`, 16 MB flash, OPI PSRAM and `USB CDC On Boot: Enabled`; upload this separate sketch.
4. The panel should show red, green and blue vertical bands, a white upper edge, a black lower edge and a small white block blinking near the lower centre. Open Serial Monitor at 115200 baud for the corresponding startup lines.
5. If the panel or ESP32 becomes hot, disconnect USB immediately and inspect power and ground. If the backlight is on but there is no pattern, verify `LCD_CS`, `LCD_RS`, `LCD_RST`, `SCK` and `SDI(MOSI)`; capture a photo and serial output before changing the controller assumption.

The test sketch does not scan keys, mount either microSD slot, access touch, or draw the final GUI. Software status is **COMPILED**; display hardware status is **UNTESTED** until the user reports the bench result.
