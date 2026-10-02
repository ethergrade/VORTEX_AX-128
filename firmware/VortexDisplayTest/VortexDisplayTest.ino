// VORTEX AX-128: first SPI LCD smoke test for a likely ST7796 module.
// Status: uploaded; no visible image reported, hardware debug in progress.
// This separate sketch tests only the LCD. Touch and the LCD's SD slot stay disconnected.

#include <Arduino.h>
#include <SPI.h>

// Temporary bench pins. The SPI pins match VortexSDTest; do not treat these
// assignments as the final PCB pinout.
constexpr uint8_t PIN_SCK = 12;
constexpr uint8_t PIN_MISO = 13;
constexpr uint8_t PIN_MOSI = 14;
constexpr uint8_t PIN_EXTERNAL_SD_CS = 21;
constexpr uint8_t PIN_LCD_CS = 1;
constexpr uint8_t PIN_LCD_RS = 38;  // DC: LOW = command, HIGH = data
constexpr uint8_t PIN_LCD_RST = 47;

constexpr uint16_t LCD_WIDTH = 480;   // landscape; native panel is 320 x 480
constexpr uint16_t LCD_HEIGHT = 320;
const SPISettings LCD_SPI_SETTINGS(10000000, MSBFIRST, SPI_MODE0);

void lcdCommand(uint8_t command, const uint8_t *data = nullptr, size_t count = 0) {
  SPI.beginTransaction(LCD_SPI_SETTINGS);
  digitalWrite(PIN_LCD_CS, LOW);
  digitalWrite(PIN_LCD_RS, LOW);
  SPI.transfer(command);
  if (count != 0) {
    digitalWrite(PIN_LCD_RS, HIGH);
    SPI.writeBytes(data, count);
  }
  digitalWrite(PIN_LCD_CS, HIGH);
  SPI.endTransaction();
}

void lcdCommand1(uint8_t command, uint8_t value) {
  lcdCommand(command, &value, 1);
}

void lcdReset() {
  digitalWrite(PIN_LCD_RST, LOW);
  delay(100);
  digitalWrite(PIN_LCD_RST, HIGH);
  delay(50);
}

void lcdInit() {
  lcdReset();

  // ST7796S IPS initialization values from LCDWiki's MSP3525/MSP3526
  // initialization reference. The exact module/controller is still to be
  // confirmed from the user's front label or a successful bench test.
  lcdCommand(0x11);  // Sleep out
  delay(120);
  lcdCommand1(0x36, 0x48);  // MADCTL, initial portrait direction
  lcdCommand1(0x3A, 0x55);  // RGB565 / 16-bit pixels
  lcdCommand1(0xF0, 0xC3);
  lcdCommand1(0xF0, 0x96);
  lcdCommand1(0xB4, 0x02);
  lcdCommand1(0xB7, 0xC6);
  const uint8_t power0[] = {0xC0, 0x00};
  lcdCommand(0xC0, power0, sizeof(power0));
  lcdCommand1(0xC1, 0x13);
  lcdCommand1(0xC2, 0xA7);
  lcdCommand1(0xC5, 0x21);
  const uint8_t displayFunction[] = {0x40, 0x8A, 0x1B, 0x1B,
                                     0x23, 0x0A, 0xAC, 0x33};
  lcdCommand(0xE8, displayFunction, sizeof(displayFunction));
  const uint8_t gammaPositive[] = {0xD2, 0x05, 0x08, 0x06, 0x05, 0x02, 0x2A,
                                   0x44, 0x46, 0x39, 0x15, 0x15, 0x2D, 0x32};
  lcdCommand(0xE0, gammaPositive, sizeof(gammaPositive));
  const uint8_t gammaNegative[] = {0x96, 0x08, 0x0C, 0x09, 0x09, 0x25, 0x2E,
                                   0x43, 0x42, 0x35, 0x11, 0x11, 0x28, 0x2E};
  lcdCommand(0xE1, gammaNegative, sizeof(gammaNegative));
  lcdCommand1(0xF0, 0x3C);
  lcdCommand1(0xF0, 0x69);
  delay(120);
  lcdCommand(0x21);         // IPS inversion on
  lcdCommand1(0x36, 0x28);  // 480 x 320 landscape, BGR order
  lcdCommand(0x29);         // Display on
}

void lcdWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  const uint8_t columns[] = {uint8_t(x0 >> 8), uint8_t(x0),
                             uint8_t(x1 >> 8), uint8_t(x1)};
  const uint8_t rows[] = {uint8_t(y0 >> 8), uint8_t(y0),
                          uint8_t(y1 >> 8), uint8_t(y1)};
  lcdCommand(0x2A, columns, sizeof(columns));
  lcdCommand(0x2B, rows, sizeof(rows));
}

void lcdFillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                 uint16_t rgb565) {
  if (x >= LCD_WIDTH || y >= LCD_HEIGHT || width == 0 || height == 0) return;
  if (width > LCD_WIDTH - x) width = LCD_WIDTH - x;
  if (height > LCD_HEIGHT - y) height = LCD_HEIGHT - y;
  lcdWindow(x, y, x + width - 1, y + height - 1);

  uint8_t pixels[128];  // 64 RGB565 pixels, no large framebuffer needed
  for (size_t i = 0; i < sizeof(pixels); i += 2) {
    pixels[i] = uint8_t(rgb565 >> 8);
    pixels[i + 1] = uint8_t(rgb565);
  }

  SPI.beginTransaction(LCD_SPI_SETTINGS);
  digitalWrite(PIN_LCD_CS, LOW);
  digitalWrite(PIN_LCD_RS, LOW);
  SPI.transfer(0x2C);  // Memory write
  digitalWrite(PIN_LCD_RS, HIGH);
  uint32_t remaining = uint32_t(width) * height;
  while (remaining != 0) {
    const uint32_t batch = remaining > 64 ? 64 : remaining;
    SPI.writeBytes(pixels, batch * 2);
    remaining -= batch;
  }
  digitalWrite(PIN_LCD_CS, HIGH);
  SPI.endTransaction();
}

void drawTestPattern() {
  // Red / green / blue bands expose data, colour-order and orientation errors.
  lcdFillRect(0, 0, 160, LCD_HEIGHT, 0xF800);
  lcdFillRect(160, 0, 160, LCD_HEIGHT, 0x07E0);
  lcdFillRect(320, 0, 160, LCD_HEIGHT, 0x001F);
  lcdFillRect(0, 0, LCD_WIDTH, 12, 0xFFFF);
  lcdFillRect(0, LCD_HEIGHT - 12, LCD_WIDTH, 12, 0x0000);
  lcdFillRect(0, 12, 12, 32, 0xFFFF);                  // top-left marker
  lcdFillRect(LCD_WIDTH - 12, LCD_HEIGHT - 44, 12, 32, 0xFFFF);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("VORTEX LCD TEST: ST7796 candidate, 480x320 landscape");
  Serial.println("Touch and display SD slot are not used in this test.");

  // Keep the separate microSD breakout inactive if it is still wired.
  pinMode(PIN_EXTERNAL_SD_CS, OUTPUT);
  digitalWrite(PIN_EXTERNAL_SD_CS, HIGH);
  pinMode(PIN_LCD_CS, OUTPUT);
  digitalWrite(PIN_LCD_CS, HIGH);
  pinMode(PIN_LCD_RS, OUTPUT);
  digitalWrite(PIN_LCD_RS, HIGH);
  pinMode(PIN_LCD_RST, OUTPUT);
  digitalWrite(PIN_LCD_RST, HIGH);

  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI);
  lcdInit();
  drawTestPattern();
  Serial.println("Pattern sent: red, green, blue; white upper edge; black lower edge.");
  Serial.println("A small white square in the lower strip will blink once per second.");
}

void loop() {
  static bool lit = false;
  lcdFillRect(LCD_WIDTH / 2 - 8, LCD_HEIGHT - 10, 16, 8,
              lit ? 0x0000 : 0xFFFF);
  lit = !lit;
  delay(1000);
}
