#pragma once

#include <Arduino.h>

#include "VortexConfig.h"

namespace vortex {

struct KeyEvent {
  uint8_t row;
  uint8_t column;
  bool pressed;
};

using KeyEventHandler = void (*)(const KeyEvent& event);

class MatrixScanner {
 public:
  void begin();
  void scan(KeyEventHandler handler);

 private:
  void selectColumn(uint8_t column);
  void updateKey(uint8_t row, uint8_t column, bool pressed, KeyEventHandler handler);

  bool rawState_[config::kMaxRowCount][config::kMaxColumnCount] = {};
  bool stableState_[config::kMaxRowCount][config::kMaxColumnCount] = {};
  uint32_t rawChangedAtMillis_[config::kMaxRowCount][config::kMaxColumnCount] = {};
};

}  // namespace vortex
