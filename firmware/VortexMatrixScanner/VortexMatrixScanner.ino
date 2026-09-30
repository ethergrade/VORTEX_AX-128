#include <Arduino.h>

#include "Keymap.h"
#include "MatrixScanner.h"
#include "VortexConfig.h"

namespace {

vortex::MatrixScanner scanner;

void printKeyEvent(const vortex::KeyEvent& event) {
  const char rowName = static_cast<char>('A' + event.row);

  Serial.printf("%c%u %s %s\n",
                rowName,
                event.column + 1,
                vortex::keyName(event.row, event.column),
                event.pressed ? "PRESSED" : "RELEASED");
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(250);

  scanner.begin();

  Serial.println("VORTEX AX-128 - MODULAR MATRIX SCANNER");
  Serial.printf("Rows: %u | Columns: %u | Debounce scans: %u\n",
                vortex::config::kActiveRowCount,
                vortex::config::kActiveColumnCount,
                vortex::config::kDebounceScanCount);
}

void loop() {
  scanner.scan(printKeyEvent);
  delay(vortex::config::kLoopDelayMillis);
}

