# TIPRO Groovebox 128
## Progetto DIY basato su ExpertKeys / Tipro EK-128

**Stato:** concept consolidato / componenti principali selezionati  
**Obiettivo:** trasformare una ExpertKeys/Tipro EK-128 in una groovebox/synth DIY ibrida digitale-analogica, costruita a mano su millefori, con sampler WAV, drum machine, FX/noise, synth, sequencer e controlli fisici.

---

## 1. Idea generale

La EK-128 diventa uno strumento autonomo composto da:

- drum sampler WAV;
- 1–2 righe dedicate a FX / noise / one-shot;
- più righe dedicate al synth;
- 16-step sequencer;
- controlli fisici a potenziometro;
- microSD per sample;
- USB per programmazione e gestione dei sample;
- uscita stereo line-level per mixer;
- uscita cuffie amplificata;
- futura sezione analogica con filtro, drive e delay.

Architettura di massima:

```text
EK-128 / matrice tasti
        │
        ▼
   ESP32-S3 N16R8
        │
        ├── microSD -> WAV / sample
        ├── PSRAM -> sample brevi precaricati
        ├── CD74HC4067 -> potenziometri
        ├── USB-C -> programmazione / gestione sample
        └── I2S
             │
             ▼
        PCM5102A DAC
             │
      ┌──────┴────────┐
      │               │
      ▼               ▼
LINE OUT L/R       TDA1308
  -> mixer            │
                      ▼
                PHONES 3.5 mm

Futura catena analogica:
PCM5102A -> buffer -> drive -> filtro -> delay PT2399 -> master
```

---

## 2. Tastiera di partenza: EK-128

Dalle foto della tastiera:

- PCB principale tasti: **PCB-1086A**
- controller separato: **PCB-T2256E**
- collegamento fra matrice e controller: **J1, 32 pin**
- USB originale: **J2**

### Strategia

Non dissaldare il controller originale.

L'obiettivo è:

1. identificare il pinout di J1;
2. verificare se J1 porta direttamente righe e colonne della matrice tasti;
3. scollegare il controller originale;
4. leggere la matrice direttamente con ESP32-S3;
5. mantenere la modifica il più possibile reversibile.

Ipotesi iniziale da verificare:

```text
8 righe x 16 colonne = 128 tasti
```

In questo caso servirebbero circa 24 linee GPIO per leggere tutta la tastiera.

---

## 3. Controller principale

### ESP32-S3 N16R8

Configurazione scelta:

- **16 MB Flash**
- **8 MB PSRAM**
- USB nativa
- I2S
- ADC
- molti GPIO
- dual core fino a 240 MHz

Funzioni previste:

- scansione matrice EK-128;
- trigger dei sample;
- sequencer;
- synth digitale;
- mixer software;
- lettura potenziometri;
- gestione microSD;
- gestione USB;
- uscita audio I2S.

---

## 4. Sample e microSD

### Modulo microSD

Scelto modulo microSD SPI tipo **Youmile GR-YM-047**.

Alimentazione prevista:

```text
3.3 V
```

Collegamento:

```text
ESP32-S3        microSD
3V3 ----------> VCC
GND ----------> GND
GPIO ---------> CS
GPIO ---------> SCK
GPIO ---------> MOSI
GPIO <--------- MISO
```

### Strategia audio

Sample brevi:

- BD
- SD
- HH
- clap
- tom
- percussioni

vengono caricati dalla SD nella **PSRAM all'avvio**.

Questo riduce la latenza al momento della pressione del tasto.

FX lunghi, drone, texture e atmosfere possono invece essere riprodotti in streaming dalla microSD.

Esempio struttura SD:

```text
/SAMPLES
    /DRUM
        01_BD.wav
        02_BD2.wav
        03_SD.wav
        04_CLAP.wav
        ...
    /FX1
        01_NOISE.wav
        02_ZAP.wav
        03_LASER.wav
        ...
    /FX2
        01_IMPACT.wav
        02_RISER.wav
        ...
    /USER
        ...
```

---

## 5. Mapping WAV sui tasti

Il firmware assocerà un file ad ogni tasto.

Esempio:

```text
KEY 113 -> /DRUM/01_BD.wav
KEY 114 -> /DRUM/02_BD2.wav
KEY 115 -> /DRUM/03_SD.wav
...
KEY 097 -> /FX1/01_NOISE.wav
```

Il sistema deve permettere più sample contemporanei:

```text
BD + HH + SD + FX
```

---

## 6. USB

Obiettivo: collegare la groovebox al Mac e sostituire i sample senza riprogrammare il firmware.

Modalità previste:

### NORMAL MODE
La groovebox funziona normalmente.

### USB STORAGE / SAMPLE MODE
Accesso ai file della microSD dal computer.

Possibile attivazione:

```text
SHIFT + USB
```

oppure combinazione tasti durante l'accensione.

Importante: evitare scrittura contemporanea sulla SD da parte di ESP32 e computer.

---

## 7. DAC audio

### PCM5102 / PCM5102A

DAC I2S stereo scelto.

Uso previsto:

```text
44.1 kHz oppure 48 kHz
16 o 24 bit
```

Collegamento concettuale:

```text
ESP32-S3       PCM5102A
GPIO BCLK ---> BCK
GPIO DATA ---> DIN
GPIO LRCK ---> LCK/LRCK
GND ---------- GND
5V/3.3V ------ VCC
```

Uscita:

```text
L / R / GND
```

Il DAC alimenta:

- line out per mixer;
- amplificatore cuffie.

---

## 8. Uscite audio

### Line Out

Prevista uscita stereo indipendente:

```text
OUT L
OUT R
GND
```

Uso:

- mixer;
- casse attive;
- interfaccia audio;
- registratore.

### Headphones

Amplificatore dedicato:

**TDA1308 stereo headphone amplifier**

Schema:

```text
PCM5102A
   │
   ├── LINE OUT L/R -> mixer
   │
   └── TDA1308
          │
          ▼
     PHONES VOL
          │
          ▼
      Jack 3.5 mm
```

Il volume cuffie deve essere indipendente dall'uscita line.

---

## 9. Multiplexer potenziometri

### CD74HC4067

Multiplexer analogico 16 canali.

Permette di leggere fino a 16 potenziometri con:

- 4 GPIO di selezione;
- 1 ingresso ADC.

Schema:

```text
POT 1  ---> C0
POT 2  ---> C1
...
POT 16 ---> C15

ESP GPIO -> S0
ESP GPIO -> S1
ESP GPIO -> S2
ESP GPIO -> S3

ESP ADC  <- SIG
EN ------ GND
VCC ----- 3.3 V
GND ----- GND
```

Aggiungere:

```text
100 nF fra VCC e GND
```

vicino al modulo.

---

## 10. Potenziometri

### Per controlli digitali

Usare preferibilmente:

```text
B10K lineari
```

Ogni potenziometro:

```text
3.3V ----[ B10K ]---- GND
             │
             └-------> ingresso CD74HC4067
```

### Kit previsti

1. kit RUNCCI con B10K;
2. kit assortito RUNCCI con:
   - B5K
   - B10K
   - B20K
   - B50K
   - B100K
   - trimmer assortiti

Il kit assortito sarà utile anche per la futura parte analogica.

---

## 11. Codifica colori dei potenziometri

La codifica visiva viene fissata così:

### Rosso = DRUM / SAMPLE
Controlli relativi a drum e sample.

### Arancio = FX
Delay, drive, feedback, FX mix e parametri effetti.

### Blu = SYNTH
Parametri timbrici principali del synth.

### Bianco = ENVELOPE / FILTER / MODULATION
Parametri di modulazione e inviluppo.

### Nero = MASTER / GENERAL
Controlli globali.

---

## 12. Disposizione proposta dei potenziometri

Predisposizione completa: **16 posizioni**.

### Layout principale

```text
DRUM / SAMPLE              SYNTH
[ROSSO] PITCH              [BLU] CUTOFF
[ROSSO] DECAY              [BLU] RESONANCE
[ROSSO] TONE               [BIANCO] ATTACK
[ROSSO] LEVEL              [BIANCO] RELEASE

FX                          MODULATION
[ARANCIO] DRIVE             [BIANCO] LFO RATE
[ARANCIO] DELAY TIME        [BIANCO] LFO DEPTH
[ARANCIO] FEEDBACK          [NERO] GLOBAL / FUTURE
[ARANCIO] FX MIX            [NERO] MASTER
```

Versione tabellare:

| # | Colore | Funzione |
|---|---|---|
| 1 | Rosso | Pitch |
| 2 | Rosso | Decay |
| 3 | Rosso | Tone |
| 4 | Rosso | Level |
| 5 | Blu | Cutoff |
| 6 | Blu | Resonance |
| 7 | Bianco | Attack |
| 8 | Bianco | Release |
| 9 | Arancio | Drive |
| 10 | Arancio | Delay Time |
| 11 | Arancio | Feedback |
| 12 | Arancio | FX Mix |
| 13 | Bianco | LFO Rate |
| 14 | Bianco | LFO Depth |
| 15 | Nero | Global / Future |
| 16 | Nero | Master |

---

## 13. Potenziometri contestuali

I potenziometri possono essere gestiti dal firmware anche in modo contestuale.

Esempio:

### Se l'ultimo elemento selezionato è una DRUM
- Pitch -> pitch sample
- Decay -> durata
- Tone -> filtro
- Level -> volume

### Se è un FX
- Pitch -> pitch
- Delay Time -> tempo
- Feedback -> feedback
- FX Mix -> mix

### Se è il SYNTH
- Pitch -> oscillator pitch/detune
- Cutoff -> filtro
- Resonance -> risonanza
- Attack / Release -> envelope
- LFO Rate / Depth -> modulazione

Questa modalità permette di ottenere molti parametri senza aumentare eccessivamente il numero di manopole.

---

## 14. Layout tasti EK-128

Proposta iniziale 8 righe x 16 tasti.

### Riga 1
Scene / Pattern / Play / Stop / Rec / Mute / Solo / Shift / utility.

### Riga 2
**16 STEP SEQUENCER**

```text
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
```

### Righe 3–5
**SYNTH**

Possibile mappatura cromatica, più funzioni:

- octave -
- octave +
- waveform
- hold
- arp
- chord
- transpose

### Riga 6
**FX / NOISE**

Possibili tasti:

```text
Noise
Zap
Laser
Reverse
Impact
Riser
Drone
Hit
Vox
Texture
Glitch
Metallic
Sweep
Atmos
FX1
FX2
```

### Riga 7
**FX / ONE SHOT / USER**

Sample liberamente sostituibili.

### Riga 8
**DRUM**

```text
BD
BD2
SD
CLAP
RIM
LT
MT
HT
CH
OH
CY
RIDE
PERC1
PERC2
COWBELL
CRASH
```

---

## 15. Synth digitale

Il synth non deve necessariamente usare WAV.

L'ESP32-S3 può generare:

- Saw
- Square
- Triangle
- Sub
- Noise

Schema logico:

```text
OSC 1 ----┐
OSC 2 ----┤
SUB ------┤ -> MIX -> FILTER -> ADSR -> VCA -> FX
NOISE ----┘
```

Prima versione:

- mono o parafonica;
- successivamente polifonica se le risorse lo permettono.

---

## 16. Sezione analogica futura

Obiettivo: mantenere il progetto realmente ibrido e non solo digitale.

Componenti previsti:

- TL072
- TL074
- PT2399
- diodi per clipping
- resistenze
- condensatori
- trimmer

Possibile catena:

```text
DAC
 │
 ▼
TL072 buffer
 │
 ▼
DRIVE
 │
 ▼
FILTER
 │
 ▼
PT2399 DELAY
 │
 ▼
FEEDBACK
 │
 ▼
MASTER
```

---

## 17. Filosofia costruttiva

Il progetto deve essere modulare.

Ordine consigliato:

1. alimentazione;
2. lettura tastiera;
3. ESP32 + test trigger;
4. microSD;
5. riproduzione WAV;
6. DAC;
7. line out;
8. cuffie;
9. potenziometri;
10. synth;
11. sequencer;
12. FX digitali;
13. FX analogici.

Ogni blocco deve funzionare autonomamente prima di aggiungere il successivo.

---

## 18. Alimentazione

Evitare collegamenti diretti alla rete 230 V all'interno del progetto.

Usare:

```text
alimentatore esterno certificato
```

e poi ricavare internamente:

- 5 V;
- 3.3 V;
- eventuale alimentazione separata per sezione analogica.

---

## 19. Step tecnico prioritario

Prima di saldare qualunque cosa sulla EK-128:

### Mappare J1 a 32 pin

Procedura:

1. scollegare completamente la tastiera dall'USB;
2. usare un multimetro in modalità continuità;
3. identificare quali coppie di pin vengono collegate premendo ogni tasto;
4. costruire una tabella righe/colonne;
5. verificare se la matrice è effettivamente circa 8x16;
6. identificare eventuali pin aggiuntivi per LED o altre funzioni;
7. solo dopo progettare il cablaggio verso ESP32.

Tabella da compilare:

```text
        C1  C2  C3  C4  ... C16
R1      K1  K2  K3  K4  ...
R2      ...
R3      ...
...
R8      ...
```

---

## 20. Componenti approvati / previsti

### Già individuati

- ExpertKeys / Tipro EK-128
- ESP32-S3 N16R8
- modulo microSD SPI
- PCM5102 / PCM5102A
- TDA1308 headphone amplifier
- CD74HC4067
- potenziometri B10K
- kit potenziometri assortiti

### Futuri

- TL072 / TL074
- PT2399
- jack stereo e mono
- potenziometro cuffie
- eventuale OLED
- rotary encoder
- connettori
- millefori
- alimentazione 5 V / 3.3 V
- condensatori di bypass
- resistenze
- switch
- knob colorati

---

## 21. Nome di lavoro

**TIPRO GROOVEBOX 128**

Possibili alternative:

- EK-128 Groove
- TIPRO-128
- EK GrooveLab
- TIPRO Noise Machine
- TIPRO Hybrid Groovebox

---

## 22. Principi progettuali fissati

- progetto reversibile dove possibile;
- niente saldature irreversibili sulla PCB principale finché non è necessario;
- sample sostituibili;
- controlli fisici immediati;
- uso intensivo dei colori;
- struttura modulare;
- facile da riparare;
- componenti economici e reperibili;
- più funzioni ottenute via firmware, senza complicare inutilmente l'hardware;
- parte analogica aggiunta progressivamente;
- documentare pinout, firmware, test, errori e modifiche ad ogni step.

---

## 23. Prossimo checkpoint

**STEP 1: reverse engineering di J1**

Obiettivo finale dello step:

```text
EK-128 J1
   │
   ▼
tabella completa RIGHE/COLONNE
   │
   ▼
mappa 128 tasti
   │
   ▼
schema collegamento ESP32-S3
```

Solo dopo questo step verrà definito il pinout definitivo dell'ESP32.


---

## 24. Step 1B/1C — Prime misure e convenzione coordinate

È stata fissata una convenzione pratica per identificare tutti i 128 tasti:

- righe fisiche: **A ... H**
- colonne fisiche: **1 ... 16**

Quindi:

```text
A1 ... A16
B1 ... B16
C1 ... C16
D1 ... D16
E1 ... E16
F1 ... F16
G1 ... G16
H1 ... H16
```

Dalle prime misure:

```text
A1 -> un terminale ha continuità con J1 pin 2
A2 -> un terminale ha continuità con J1 pin 4
```

### Ipotesi di lavoro

È possibile che i 16 pin pari di J1 rappresentino le 16 colonne:

```text
Colonna 1  -> J1 pin 2
Colonna 2  -> J1 pin 4
Colonna 3  -> J1 pin 6
...
Colonna 16 -> J1 pin 32
```

Questa ipotesi NON è ancora considerata confermata.

### Test successivo più efficiente

Verificare:

```text
A16, terminale già appartenente alla colonna -> J1 pin 32 ?
```

Poi individuare il secondo terminale di:

```text
A1
A2
A16
```

Se tutti e tre hanno continuità verso lo stesso pin J1, quel pin sarà con alta probabilità la linea **ROW A**.

Successivamente misurare il secondo terminale di:

```text
B1
C1
D1
E1
F1
G1
H1
```

per identificare le otto linee di riga.

Tabella da compilare:

| Riga | Tasto test | Pin colonna | Pin riga |
|---|---|---:|---:|
| A | A1 | 2 | ? |
| A | A2 | 4 | ? |
| A | A16 | 32? | ? |
| B | B1 | 2? | ? |
| C | C1 | 2? | ? |
| D | D1 | 2? | ? |
| E | E1 | 2? | ? |
| F | F1 | 2? | ? |
| G | G1 | 2? | ? |
| H | H1 | 2? | ? |

### Nota importante

Prima di collegare la matrice direttamente all'ESP32-S3 dovrà essere verificato anche il comportamento con pressioni multiple, per capire se la tastiera possiede diodi per-tasto o se esiste rischio di ghosting durante accordi, drum hit simultanei o combinazioni di più tasti.


### Step 1D — Misura A16

Nuova misura confermata:

```text
A16:
- terminale colonna -> J1 pin 32
- altro terminale   -> J1 pin 5
```

Questo rafforza fortemente l'ipotesi:

```text
Colonna 1  -> J1 pin 2
Colonna 2  -> J1 pin 4
...
Colonna 16 -> J1 pin 32
```

e suggerisce:

```text
Riga A -> J1 pin 5
```

La riga A sarà considerata confermata quando anche il secondo terminale di A1 e/o A2 risulterà su J1 pin 5.
