#include <Arduino.h>

#include "Keymap.h"
#include "MatrixScanner.h"
#include "VortexConfig.h"

namespace {

vortex::MatrixScanner scanner;
bool tested[vortex::config::kMaxRowCount][vortex::config::kMaxColumnCount] = {};
uint16_t testedCount = 0;

constexpr uint16_t kTotalKeys =
    vortex::config::kActiveRowCount * vortex::config::kActiveColumnCount;

void printMapping(uint8_t row, uint8_t column) {
  Serial.printf("%c%d | %s | riga J1-%d/GPIO%d | colonna J1-%d/C%d",
                static_cast<char>('A' + row),
                column + 1,
                vortex::keyName(row, column),
                3 + 2 * row,
                vortex::config::kRowPins[row],
                2 * (column + 1),
                column);
}

void printKeyEvent(const vortex::KeyEvent& event) {
  printMapping(event.row, event.column);
  Serial.print(event.pressed ? " | PREMUTO" : " | RILASCIATO");

  bool completedNow = false;
  if (event.pressed && !tested[event.row][event.column]) {
    tested[event.row][event.column] = true;
    ++testedCount;
    Serial.printf(" | TESTATI %d/%d", testedCount, kTotalKeys);
    completedNow = testedCount == kTotalKeys;
  }

  Serial.println();
  if (completedNow) {
    Serial.println("Tutte le 128 coordinate sono state premute almeno una volta.");
  }
}

void printMissing() {
  Serial.printf("Testati: %d/%d. Mancanti:\n", testedCount, kTotalKeys);
  if (testedCount == kTotalKeys) {
    Serial.println("Nessuno.");
    return;
  }

  for (uint8_t row = 0; row < vortex::config::kActiveRowCount; ++row) {
    for (uint8_t column = 0; column < vortex::config::kActiveColumnCount; ++column) {
      if (!tested[row][column]) {
        printMapping(row, column);
        Serial.println();
      }
    }
  }
}

void printFullMap() {
  for (uint8_t row = 0; row < vortex::config::kActiveRowCount; ++row) {
    for (uint8_t column = 0; column < vortex::config::kActiveColumnCount; ++column) {
      printMapping(row, column);
      Serial.println();
    }
  }
}

void handleSerial() {
  while (Serial.available() > 0) {
    const char command = static_cast<char>(Serial.read());
    switch (command) {
      case 'm':
      case 'M':
        printMissing();
        break;
      case 'p':
      case 'P':
        printFullMap();
        break;
      case 'r':
      case 'R':
        for (uint8_t row = 0; row < vortex::config::kActiveRowCount; ++row) {
          for (uint8_t column = 0; column < vortex::config::kActiveColumnCount;
               ++column) {
            tested[row][column] = false;
          }
        }
        testedCount = 0;
        Serial.println("Conteggio azzerato. Rilascia e ripremi i tasti ancora premuti.");
        break;
      case '?':
        Serial.println("Comandi: m = mancanti, p = mappa completa, r = azzera conteggio.");
        break;
      default:
        break;
    }
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(250);

  scanner.begin();

  Serial.println("VORTEX AX-128 - TEST MATRICE EK-128");
  Serial.printf("Righe: %d | Colonne: %d | Antirimbalzo: %d ms\n",
                vortex::config::kActiveRowCount,
                vortex::config::kActiveColumnCount,
                vortex::config::kDebounceMillis);
  Serial.println("Premi ogni tasto una volta. Comandi: m, p, r, ?");
}

void loop() {
  handleSerial();
  scanner.scan(printKeyEvent);
  delay(vortex::config::kLoopDelayMillis);
}
