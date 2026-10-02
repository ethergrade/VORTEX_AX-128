# 21 — First SPI display wiring and smoke test

## Identification and scope

The user's rear photo [`IMG_8864_no_metadata.jpg`](../assets/Displays/IMG_8864_no_metadata.jpg) shows a 14-pin SPI TFT module with capacitive-touch flex, an onboard microSD socket, a 74LVC245A buffer and a small onboard regulator. Its labels and component layout match the LCDWiki MSP3526 / Elecrow DLS30035B family, which uses an ST7796-class 320 × 480 panel. **The exact model and front-side size marking have not yet been independently confirmed.** The first test is therefore a cautious LCD-only smoke test for this identified board family; the final two-display arrangement remains open.

The manufacturer's [module page](https://www.lcdwiki.com/3.5inch_IPS_SPI_Module_ST7796) specifies a 5 V working supply and approximately 0.5 W for the matching 3.5-inch module. Its [schematic](https://www.lcdwiki.com/res/MSP3525_MSP3526/3.5inch_SPI_Module_MSP3525_MSP3526_Schematic.pdf) shows the regulator, SPI buffers and touch I²C level-shifting circuit. The manufacturer also states that the `LED` pin may be left open for an enabled backlight. The rear silkscreen on the actual board remains the physical authority for pin order.

The Freenove base visible in the board photos is breakout v1.2 and is marked `Input: 7-12V`. Its barrel input feeds regulated 5V0 and 3.3V rails. The red power rails are configurable separately through jumpers `VC1` and `VC2`; each may select `5V0` or `3V3`. For this LCD VCC, set the specific rail used by its red lead to `5V0`, and use the black rail for GND. The Freenove [FNK0091 documentation](https://docs.freenove.com/projects/fnk0091/en/latest/fnk0091/codes/tutorial/1_Freenove_Breakout_Board.html) documents the input range and selectable rails; the [power precautions](https://docs.freenove.com/projects/fnk0101/en/latest/fnk0101/codes/tutorial/2_ESP32.html) specify a 5.5 x 2.1 mm DC jack and warn to check polarity. The board manual recommends isolating the positive 5 V link between the ESP32 board and breakout if USB and external DC are connected at the same time. For a standalone test, upload via USB first, disconnect USB, then power from the DC jack.

**Disconnect USB power before wiring.** Check the display's exact product label or listing before applying power if it differs from this board family. The display `VCC` goes to the Freenove **`5V`** rail for this matching module; the separate six-pin microSD breakout is marked **`3V3`** and must still go only to **`3V3`**. Both share GND. Never exchange these two power rails.

## Complete 14-pin rear-header map

With the **rear as photographed**, the header is at left; the top pin is `SD_CS` and the bottom pin is `VCC`. Number 1 is therefore at the **bottom**. Use the printed labels if the board is turned around.

| Rear top → bottom | Header pin | Display label | Freenove ESP32-S3, first test | Meaning |
|---:|---:|---|---|---|
| 1 | 14 | `SD_CS` | `GPIO47` | Selects the display's *own* microSD slot; held high during the LCD-only test |
| 2 | 13 | `CTP_INT` | Leave open | Capacitive-touch interrupt; unused |
| 3 | 12 | `CTP_SDA` | Leave open | Capacitive-touch I²C data; unused |
| 4 | 11 | `CTP_RST` | Leave open | Capacitive-touch reset; unused |
| 5 | 10 | `CTP_SCL` | Leave open | Capacitive-touch I²C clock; unused |
| 6 | 9 | `SDO(MISO)` | `GPIO13` | Shared SPI data back to ESP32 |
| 7 | 8 | `LED` | Leave open | Backlight defaults on for matching module |
| 8 | 7 | `SCK` | `GPIO12` | Shared SPI clock |
| 9 | 6 | `SDI(MOSI)` | `GPIO14` | Shared SPI data from ESP32 |
| 10 | 5 | `LCD_RS` | `GPIO38` | Command/data selection (`DC`) |
| 11 | 4 | `LCD_RST` | `GPIO21` | Display reset |
| 12 | 3 | `LCD_CS` | `GPIO1` | Display chip select, active low |
| 13 | 2 | `GND` | `GND` | Common ground |
| 14 | 1 | `VCC` | `5V` | Display-module power for matching 5 V board |

These are **temporary bench GPIOs**. The user confirmed the actual display reset wire is `GPIO21`; the display's own `SD_CS` uses `GPIO47`. The LCD test holds both chip-select lines high. The separate six-pin SD breakout remains disconnected while testing the display slot.

With display `VCC = 5V`, the matching schematic pulls the external touch I²C lines up to `VCC`. **Do not attach `CTP_SCL` or `CTP_SDA` directly to 3.3 V ESP32 GPIOs in this configuration.** Touch needs a separately checked level-safe wiring plan. Leave all four `CTP_*` pins open now. The matching schematic says `LED` open leaves the backlight on; an unlit panel alone does not prove SPI is working.

## First test

1. Leave the working EK-128/multiplexer wiring alone. With USB unplugged, connect the display wires shown above: `VCC`, `GND`, `LCD_CS`, `LCD_RST`, `LCD_RS`, `SDI`, `SCK`, `SDO`, and `SD_CS`. `LED` and touch pins remain open.
2. Recheck the *physical* top and bottom labels before powering the board; ensure `5V` goes only to display `VCC` and `3V3` only to the separate microSD module.
3. In Arduino IDE open `VortexDisplayTest/VortexDisplayTest.ino`. Select `ESP32S3 Dev Module`, 16 MB flash, OPI PSRAM and `USB CDC On Boot: Disabled` for the currently connected USB-to-UART port; upload this separate sketch.
4. The panel should show red, green and blue vertical bands, a white upper edge, a black lower edge and a small white block blinking near the lower centre. Open Serial Monitor at 115200 baud for the corresponding startup lines.
5. If the panel or ESP32 becomes hot, disconnect USB immediately and inspect power and ground. If the backlight is on but there is no pattern, verify `LCD_CS`, `LCD_RS`, `LCD_RST`, `SCK` and `SDI(MOSI)`; capture a photo and serial output before changing the controller assumption.

The test sketch does not scan keys, mount the microSD slot, access touch, or draw the final GUI. Software status is **COMPILED AND UPLOADED**; the user reports that the display works. The physical reset connection is `GPIO21`, and `GPIO47` is reserved for the display-slot `SD_CS`.
