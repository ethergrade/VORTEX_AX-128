#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <esp_heap_caps.h>

#include <cstring>

namespace {

#define VORTEX_SD_SOURCE_DISPLAY_SLOT 1

// Temporary SPI test pins on the Freenove ESP32-S3, not a frozen project pinout.
// They do not overlap the EK-128 scanner GPIOs documented in docs/03.
constexpr int kSckPin = 12;
constexpr int kMisoPin = 13;
constexpr int kMosiPin = 14;
constexpr int kLcdCsPin = 1;
#if VORTEX_SD_SOURCE_DISPLAY_SLOT
constexpr int kCsPin = 47;
constexpr int kLcdRstPin = 21;
#else
constexpr int kCsPin = 21;
constexpr int kLcdRstPin = -1;
#endif
constexpr uint32_t kSpiHz = 4000000;
constexpr size_t kMaxPsrLoadBytes = 64 * 1024;
constexpr char kSampleDirectory[] = "/samples";

struct WavInfo {
  uint16_t format = 0;
  uint16_t channels = 0;
  uint32_t sampleRate = 0;
  uint16_t bitsPerSample = 0;
  uint32_t dataOffset = 0;
  uint32_t dataBytes = 0;
};

uint16_t little16(const uint8_t* p) {
  return static_cast<uint16_t>(p[0]) |
         (static_cast<uint16_t>(p[1]) << 8);
}

uint32_t little32(const uint8_t* p) {
  return static_cast<uint32_t>(p[0]) |
         (static_cast<uint32_t>(p[1]) << 8) |
         (static_cast<uint32_t>(p[2]) << 16) |
         (static_cast<uint32_t>(p[3]) << 24);
}

bool readExactly(File& file, uint8_t* buffer, size_t length) {
  size_t total = 0;
  while (total < length) {
    const size_t count = file.read(buffer + total, length - total);
    if (count == 0) {
      return false;
    }
    total += count;
  }
  return true;
}

bool hasWavExtension(const char* name) {
  const char* dot = strrchr(name, '.');
  if (dot == nullptr || strlen(dot) != 4) {
    return false;
  }
  return (dot[1] == 'w' || dot[1] == 'W') &&
         (dot[2] == 'a' || dot[2] == 'A') &&
         (dot[3] == 'v' || dot[3] == 'V');
}

bool inspectWav(File& file, WavInfo& info) {
  const size_t fileSize = file.size();
  if (fileSize < 12 || fileSize > UINT32_MAX || !file.seek(0)) {
    return false;
  }

  uint8_t riff[12];
  if (!readExactly(file, riff, sizeof(riff)) ||
      memcmp(riff, "RIFF", 4) != 0 || memcmp(riff + 8, "WAVE", 4) != 0) {
    return false;
  }

  bool foundFormat = false;
  bool foundData = false;
  uint32_t position = 12;
  while (position <= fileSize - 8) {
    uint8_t chunk[8];
    if (!file.seek(position) || !readExactly(file, chunk, sizeof(chunk))) {
      return false;
    }

    const uint32_t chunkBytes = little32(chunk + 4);
    const uint32_t payload = position + 8;
    const uint64_t next = static_cast<uint64_t>(payload) + chunkBytes +
                          (chunkBytes & 1U);
    if (next > fileSize) {
      return false;
    }

    if (memcmp(chunk, "fmt ", 4) == 0 && !foundFormat) {
      if (chunkBytes < 16 || !file.seek(payload)) {
        return false;
      }
      uint8_t format[16];
      if (!readExactly(file, format, sizeof(format))) {
        return false;
      }
      info.format = little16(format);
      info.channels = little16(format + 2);
      info.sampleRate = little32(format + 4);
      info.bitsPerSample = little16(format + 14);
      foundFormat = true;
    } else if (memcmp(chunk, "data", 4) == 0 && !foundData) {
      info.dataOffset = payload;
      info.dataBytes = chunkBytes;
      foundData = true;
    }

    if (foundFormat && foundData) {
      break;
    }
    position = static_cast<uint32_t>(next);
  }

  return foundFormat && foundData && info.format == 1 &&
         (info.channels == 1 || info.channels == 2) &&
         info.sampleRate > 0 && info.bitsPerSample == 16 &&
         info.dataBytes > 0;
}

bool loadFragmentToPsram(File& file, const WavInfo& info) {
  if (!psramFound()) {
    Serial.println("PSRAM non rilevata: controllo Tools > PSRAM: OPI PSRAM.");
    return false;
  }

  const size_t length = info.dataBytes < kMaxPsrLoadBytes ?
                            info.dataBytes :
                            kMaxPsrLoadBytes;
  auto* buffer = static_cast<uint8_t*>(
      heap_caps_malloc(length, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
  if (buffer == nullptr) {
    Serial.println("Allocazione PSRAM fallita.");
    return false;
  }

  size_t total = 0;
  if (file.seek(info.dataOffset)) {
    while (total < length) {
      const size_t remaining = length - total;
      const size_t request = remaining < 4096 ? remaining : 4096;
      const size_t count = file.read(buffer + total, request);
      if (count == 0) {
        break;
      }
      total += count;
    }
  }

  uint32_t checksum = 2166136261U;
  for (size_t i = 0; i < total; ++i) {
    checksum = (checksum ^ buffer[i]) * 16777619U;
  }
  heap_caps_free(buffer);

  if (total != length) {
    Serial.printf("Lettura incompleta: %u/%u byte.\n",
                  static_cast<unsigned>(total),
                  static_cast<unsigned>(length));
    return false;
  }
  Serial.printf("PSRAM OK: %u byte WAV copiati, checksum FNV-1a %08lX\n",
                static_cast<unsigned>(total),
                static_cast<unsigned long>(checksum));
  return true;
}

void runTest() {
  Serial.println("VORTEX AX-128 - TEST microSD SPI / WAV / PSRAM");
  Serial.printf("SPI: SCK GPIO%d, MISO GPIO%d, MOSI GPIO%d, SD_CS GPIO%d; "
                "LCD_CS GPIO%d disattivato",
                kSckPin, kMisoPin, kMosiPin, kCsPin, kLcdCsPin);
  if (kLcdRstPin >= 0) {
    Serial.printf("; LCD_RST GPIO%d mantenuto alto\n", kLcdRstPin);
  } else {
    Serial.println();
  }

  pinMode(kLcdCsPin, OUTPUT);
  digitalWrite(kLcdCsPin, HIGH);
  if (kLcdRstPin >= 0) {
    pinMode(kLcdRstPin, OUTPUT);
    digitalWrite(kLcdRstPin, HIGH);
  }
  pinMode(kCsPin, OUTPUT);
  digitalWrite(kCsPin, HIGH);
  SPI.begin(kSckPin, kMisoPin, kMosiPin, kCsPin);
  if (!SD.begin(kCsPin, SPI, kSpiHz)) {
    Serial.println("microSD non montata: controlla modulo, scheda, FAT32 e fili SPI.");
    return;
  }

  const uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("Nessuna scheda inserita.");
    return;
  }
  Serial.printf("Scheda rilevata. Capacita: %lu MB. PSRAM libera: %lu KB\n",
                static_cast<unsigned long>(SD.cardSize() / (1024 * 1024)),
                static_cast<unsigned long>(ESP.getFreePsram() / 1024));

  File directory = SD.open(kSampleDirectory);
  if (!directory || !directory.isDirectory()) {
    Serial.println("Cartella /samples assente. Creala sulla scheda dal computer.");
    return;
  }

  uint16_t wavCount = 0;
  uint16_t pcm16Count = 0;
  bool psramLoaded = false;
  File file = directory.openNextFile();
  while (file) {
    if (!file.isDirectory() && hasWavExtension(file.name())) {
      ++wavCount;
      Serial.printf("WAV %d: %s (%lu byte)\n",
                    wavCount, file.name(),
                    static_cast<unsigned long>(file.size()));
      WavInfo info;
      if (inspectWav(file, info)) {
        ++pcm16Count;
        Serial.printf("  PCM16: %d canale/i, %lu Hz, %lu byte audio\n",
                      info.channels,
                      static_cast<unsigned long>(info.sampleRate),
                      static_cast<unsigned long>(info.dataBytes));
        if (!psramLoaded) {
          psramLoaded = loadFragmentToPsram(file, info);
        }
      } else {
        Serial.println("  Formato WAV non valido o non PCM 16 bit mono/stereo.");
      }
    }
    file.close();
    file = directory.openNextFile();
  }
  directory.close();
  Serial.printf("Risultato: %d WAV, %d PCM16 compatibili, PSRAM %s.\n",
                wavCount, pcm16Count, psramLoaded ? "OK" : "NON VERIFICATA");
}

}  // namespace

void setup() {
  Serial.begin(115200);
  // Give the USB-to-UART serial monitor time to reconnect after reset.
  delay(1500);
  runTest();
}

void loop() {
  delay(1000);
}
