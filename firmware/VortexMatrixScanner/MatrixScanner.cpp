#include "MatrixScanner.h"

namespace vortex {

void MatrixScanner::begin() {
  for (uint8_t pin : config::kMuxSelectPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  for (uint8_t row = 0; row < config::kActiveRowCount; ++row) {
    pinMode(config::kRowPins[row], INPUT_PULLUP);
  }
}

void MatrixScanner::scan(KeyEventHandler handler) {
  for (uint8_t column = 0; column < config::kActiveColumnCount; ++column) {
    selectColumn(column);

    for (uint8_t row = 0; row < config::kActiveRowCount; ++row) {
      const bool pressed = digitalRead(config::kRowPins[row]) == LOW;
      updateKey(row, column, pressed, handler);
    }
  }
}

void MatrixScanner::selectColumn(uint8_t column) {
  for (uint8_t bit = 0; bit < 4; ++bit) {
    digitalWrite(config::kMuxSelectPins[bit], (column >> bit) & 0x01U);
  }

  delayMicroseconds(config::kMuxSettleTimeMicros);
}

void MatrixScanner::updateKey(uint8_t row,
                              uint8_t column,
                              bool pressed,
                              KeyEventHandler handler) {
  if (pressed != rawState_[row][column]) {
    rawState_[row][column] = pressed;
    stableScanCount_[row][column] = 1;
  } else if (stableScanCount_[row][column] < config::kDebounceScanCount) {
    ++stableScanCount_[row][column];
  }

  const bool isStable = stableScanCount_[row][column] >= config::kDebounceScanCount;
  if (!isStable || stableState_[row][column] == rawState_[row][column]) {
    return;
  }

  stableState_[row][column] = rawState_[row][column];
  if (handler != nullptr) {
    handler(KeyEvent{row, column, stableState_[row][column]});
  }
}

}  // namespace vortex

