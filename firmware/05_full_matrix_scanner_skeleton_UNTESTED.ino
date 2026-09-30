// VORTEX AX-128
// FULL MATRIX SCANNER SKELETON
// NOT YET VALIDATED WITH ALL 128 KEYS.
// Uses frozen Rev.A pinout.

const uint8_t muxS[4] = {4, 6, 7, 15};
const uint8_t rowPins[8] = {5, 16, 17, 18, 8, 9, 10, 11};

bool keyState[8][16] = {};

void selectColumn(uint8_t col) {
  digitalWrite(muxS[0], col & 0x01);
  digitalWrite(muxS[1], (col >> 1) & 0x01);
  digitalWrite(muxS[2], (col >> 2) & 0x01);
  digitalWrite(muxS[3], (col >> 3) & 0x01);
  delayMicroseconds(10);
}

void setup() {
  Serial.begin(115200);

  for (uint8_t i = 0; i < 4; ++i) {
    pinMode(muxS[i], OUTPUT);
  }

  for (uint8_t r = 0; r < 8; ++r) {
    pinMode(rowPins[r], INPUT_PULLUP);
  }

  Serial.println("VORTEX AX-128 - FULL MATRIX SKELETON");
}

void loop() {
  for (uint8_t col = 0; col < 16; ++col) {
    selectColumn(col);

    for (uint8_t row = 0; row < 8; ++row) {
      bool pressed = digitalRead(rowPins[row]) == LOW;

      if (pressed != keyState[row][col]) {
        keyState[row][col] = pressed;

        char rowName = 'A' + row;

        Serial.printf("%c%d %s\n",
                      rowName,
                      col + 1,
                      pressed ? "PRESSED" : "RELEASED");
      }
    }
  }

  delay(1);
}
