# ESP32-S3 — Struttura modulare C/C++ con file esterni e librerie

È assolutamente possibile realizzare un progetto ESP32-S3 in C/C++ suddiviso in più file `.cpp`, `.h`, eventuali `.c`, liste esterne, configurazioni e librerie, mantenendo tutto dentro una singola cartella di progetto.

Per un progetto complesso è anzi l'approccio consigliato.

---

## 1. Struttura base del progetto

Esempio:

```text
VORTEX_AX128/
│
├── src/
│   ├── main.cpp
│   ├── Core.cpp
│   ├── Master_Control.cpp
│   ├── AudioEngine.cpp
│   ├── FilterControl.cpp
│   └── MidiEngine.cpp
│
├── include/
│   ├── Core.h
│   ├── a.h
│   ├── Master_Control.h
│   ├── AudioEngine.h
│   ├── FilterControl.h
│   └── MidiEngine.h
│
├── data/
│   ├── presets.txt
│   ├── waves.txt
│   └── config.json
│
├── lib/
│   └── MiaLibreria/
│       ├── MiaLibreria.cpp
│       └── MiaLibreria.h
│
└── platformio.ini
```

---

# 2. Esempio: Core.cpp che usa un file header esterno

## Core.cpp

```cpp
#include "Core.h"
#include "a.h"

void Core_Init()
{
    inizializzaA();
}
```

## a.h

```cpp
#pragma once

void inizializzaA();
```

## a.cpp

```cpp
#include "a.h"

void inizializzaA()
{
    // codice
}
```

Schema logico:

```text
Core.cpp
   │
   └──> a.h
          │
          └──> a.cpp
```

---

# 3. Master_Control.cpp che utilizza liste esterne

È possibile avere file separati contenenti liste di preset, strumenti, wave, sample, configurazioni, ecc.

## Master_Control.cpp

```cpp
#include "Master_Control.h"
#include "PresetList.h"
#include "WaveList.h"

void MasterControl_Init()
{
    int preset = presetList[0];
}
```

---

# 4. Liste esterne: usare `extern`

Per liste grandi è preferibile NON inserire direttamente tutti i dati dentro il file `.h`.

Meglio:

## PresetList.h

```cpp
#pragma once

extern const int presetList[];
```

## PresetList.cpp

```cpp
#include "PresetList.h"

const int presetList[] = {
    10,
    20,
    30,
    40
};
```

## Utilizzo

```cpp
#include "PresetList.h"

int value = presetList[0];
```

Schema:

```text
PresetList.h
      │
      │ dichiarazione
      ↓
PresetList.cpp
      │
      │ dati reali
      ↓
Master_Control.cpp
      │
      └── utilizza i dati
```

Questo evita duplicazioni e problemi di linking.

---

# 5. Liste strutturate di strumenti / sample

È possibile creare strutture dati più evolute.

## InstrumentList.h

```cpp
#pragma once

struct Instrument {
    const char* name;
    const char* filename;
    int rootNote;
};

extern const Instrument instruments[];
extern const int instrumentCount;
```

## InstrumentList.cpp

```cpp
#include "InstrumentList.h"

const Instrument instruments[] = {
    {"Analog Strings", "/samples/strings.wav", 60},
    {"Juno Pad",       "/samples/juno.wav",    60},
    {"Dark Choir",     "/samples/choir.wav",   57},
    {"OB Brass",       "/samples/brass.wav",   60}
};

const int instrumentCount =
    sizeof(instruments) / sizeof(instruments[0]);
```

## Utilizzo

```cpp
#include "InstrumentList.h"

void selectInstrument(int index)
{
    const Instrument& inst = instruments[index];

    loadSample(inst.filename);
}
```

---

# 6. Usare file C insieme a C++

ESP32-S3 permette di mischiare file:

```text
.cpp
.h
.c
```

nello stesso progetto.

Esempio:

```text
src/
    main.cpp
    Core.cpp
    dsp.c

include/
    dsp.h
```

## dsp.c

```c
#include "dsp.h"

float process_sample(float input)
{
    return input * 0.5f;
}
```

## dsp.h

```c
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

float process_sample(float input);

#ifdef __cplusplus
}
#endif
```

## Chiamata da C++

```cpp
#include "dsp.h"

float value = process_sample(0.8f);
```

Il blocco:

```cpp
#ifdef __cplusplus
extern "C" {
#endif
```

permette al codice C++ di chiamare correttamente funzioni compilate in C.

---

# 7. Struttura consigliata per VORTEX AX-128

Per un progetto complesso conviene suddividere il firmware per responsabilità.

```text
VORTEX_AX128/
│
├── src/
│   ├── main.cpp
│   │
│   ├── core/
│   │   ├── Core.cpp
│   │   ├── MasterControl.cpp
│   │   └── SystemState.cpp
│   │
│   ├── audio/
│   │   ├── AudioEngine.cpp
│   │   ├── Voice.cpp
│   │   ├── Layer.cpp
│   │   └── Mixer.cpp
│   │
│   ├── midi/
│   │   └── MidiEngine.cpp
│   │
│   ├── control/
│   │   ├── Knobs.cpp
│   │   ├── Buttons.cpp
│   │   └── Encoder.cpp
│   │
│   └── storage/
│       ├── PresetManager.cpp
│       └── SampleManager.cpp
│
├── include/
│   ├── Core.h
│   ├── MasterControl.h
│   ├── AudioEngine.h
│   ├── Voice.h
│   ├── Layer.h
│   ├── MidiEngine.h
│   ├── PresetManager.h
│   └── VortexConfig.h
│
├── lib/
│   ├── AnalogFilterControl/
│   └── CustomDSP/
│
├── data/
│   ├── presets/
│   ├── samples/
│   └── config/
│
└── platformio.ini
```

---

# 8. main.cpp minimale

Il file `main.cpp` dovrebbe restare il più semplice possibile.

```cpp
#include <Arduino.h>

#include "Core.h"
#include "AudioEngine.h"
#include "MasterControl.h"
#include "MidiEngine.h"

void setup()
{
    Core::begin();
    AudioEngine::begin();
    MidiEngine::begin();
    MasterControl::begin();
}

void loop()
{
    Core::update();
    MidiEngine::update();
    MasterControl::update();
    AudioEngine::update();
}
```

In questo modo `main.cpp` non diventa un file enorme da migliaia di righe.

---

# 9. File di configurazione globale

È utile avere un header centrale per costanti hardware e software.

## VortexConfig.h

```cpp
#pragma once

#define NUM_LAYERS 4
#define NUM_VOICES 16

#define SAMPLE_RATE 48000

#define PIN_FILTER_CUTOFF_1 4
#define PIN_FILTER_CUTOFF_2 5
#define PIN_FILTER_CUTOFF_3 6
#define PIN_FILTER_CUTOFF_4 7
```

Ogni modulo può quindi includerlo:

```cpp
#include "VortexConfig.h"
```

---

# 10. Librerie esterne

È possibile usare contemporaneamente:

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <USB.h>
#include <MIDI.h>

#include "Core.h"
#include "AudioEngine.h"
```

Con PlatformIO le dipendenze possono essere dichiarate direttamente nel file:

```text
platformio.ini
```

Esempio:

```ini
[env:esp32-s3]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

lib_deps =
    fortyseveneffects/MIDI Library
    bblanchon/ArduinoJson
```

PlatformIO scarica e compila automaticamente le librerie richieste.

---

# 11. Architettura consigliata

È meglio evitare una catena di dipendenze troppo lunga:

```text
Core
  ↓
A
  ↓
B
  ↓
C
  ↓
D
```

Meglio organizzare il software per moduli indipendenti:

```text
                 main.cpp
                     │
        ┌────────────┼─────────────┐
        ↓            ↓             ↓
      CORE         AUDIO        CONTROL
        │            │             │
        ↓            ↓             ↓
     SYSTEM        LAYERS         PANEL
                     │
              ┌──────┼──────┐
              ↓      ↓      ↓
           SAMPLES   DSP   FILTER
```

Ogni modulo dovrebbe avere una responsabilità precisa.

---

# 12. Esempio concettuale VORTEX

Possibile suddivisione:

```text
Core
├── gestione sistema
├── scheduler
├── stato globale
└── inizializzazione

MasterControl
├── gestione layer
├── preset
├── routing
├── controllo parametri
└── stato pannello

AudioEngine
├── playback sample
├── voice allocation
├── mixer
├── DSP
└── output audio

MidiEngine
├── MIDI IN
├── MIDI OUT
├── MIDI USB
├── clock
└── mapping controlli

FilterControl
├── cutoff Layer 1
├── cutoff Layer 2
├── cutoff Layer 3
├── cutoff Layer 4
├── global cutoff
└── global resonance

Storage
├── SD
├── preset
├── sample
├── wave
└── configurazioni
```

---

# 13. Conclusione

Un progetto ESP32-S3 può tranquillamente essere composto da:

```text
.cpp
.h
.c
.json
.txt
.csv
.bin
.wav
```

e da librerie esterne.

Tutto può essere contenuto in una singola cartella di progetto e compilato in un unico firmware.

Per un progetto complesso come VORTEX AX-128 è consigliato utilizzare:

```text
VS Code
+
PlatformIO
+
ESP32-S3
+
C/C++
```

anziché mantenere tutto dentro un singolo file `.ino`.

La struttura modulare permette di sviluppare, testare e sostituire singole parti del firmware senza dover modificare tutto il progetto.
