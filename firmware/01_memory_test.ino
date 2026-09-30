#include "esp_system.h"

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println();
  Serial.println("================================");
  Serial.println("       VORTEX AX-128 TEST");
  Serial.println("================================");

  Serial.printf("CPU:           %d MHz\n", getCpuFrequencyMhz());
  Serial.printf("Flash:         %u MB\n",
                ESP.getFlashChipSize() / (1024 * 1024));

  Serial.printf("PSRAM totale:  %u MB\n",
                ESP.getPsramSize() / (1024 * 1024));

  Serial.printf("PSRAM libera:  %u KB\n",
                ESP.getFreePsram() / 1024);

  Serial.printf("RAM libera:    %u KB\n",
                ESP.getFreeHeap() / 1024);

  Serial.println(psramFound() ? "PSRAM:         OK" : "PSRAM:         NON RILEVATA");
  Serial.println("================================");
}

void loop() {
  Serial.printf("VORTEX alive | uptime: %lu s | free PSRAM: %u KB\n",
                millis() / 1000,
                ESP.getFreePsram() / 1024);
  delay(2000);
}
