# 19 — ESP32 audio, mixer e controlli: studio di fattibilità

Ricerca del 2026-10-01. Questa è una proposta tecnica, non modifica le decisioni congelate in `02_FROZEN_DECISIONS.md`. Il firmware audio e i nuovi circuiti qui descritti non sono ancora stati provati su VORTEX.

## Base VORTEX verificata

- ESP32-S3 Freenove N16R8, quattro layer simultanei, due PCM5102A stereo per quattro uscite mono, un filtro analogico indipendente per layer.
- La tastiera EK-128 è mappata 8 × 16. Aggiornamento del 2026-10-02: l'utente ha riferito eventi di pressione e rilascio per tutti i 128 tasti con lo scanner CD74HC4067; restano da verificare la corrispondenza fisica dei tasti, le pressioni simultanee tramite mux e la temperatura del modulo.
- Riverbero lungo per layer: requisito del progetto; partire da algoritmi compatti e misurare prima di promettere quattro istanze complete.
- IC posseduti elencati in `11_COMPONENT_INVENTORY_BOM.md`: in particolare NE5532, LM324, LM358, LM393. L'utente ha inoltre riferito di possedere altri quattro CD74HC4067; nella working copy c'è un datasheet MCP3008, ma la disponibilità fisica del chip non è confermata dal solo datasheet.
- Nessuno slider. Manopole con potenziometro rotativo per funzioni sempre visibili; encoder rotativi a rotazione continua, possibilmente con pressione, solo per parametri di pagina.

## Fonti esaminate e decisione

| Fonte | Evidenza nel codice/materiale | Impiego VORTEX |
|---|---|---|
| [ESP32Synth](https://github.com/danilogcrf2-oss/ESP32Synth), commit `75ddb19`, MIT | Voci a punto fisso, streaming WAV, wavetable, callback DSP, quattro bus configurabili, esempio di reverb; `render()` somma i bus in bus 0, `generateSamplesStereo()` duplica il mono. | Buona base di studio per voci e rendering efficiente. Un fork richiede uscita a quattro buffer indipendenti e trasporto su due I2S; i bus attuali non sono già quattro uscite fisiche. |
| [ESP32 SF2 Sampler](https://github.com/copych/ESP32_SF2_Sampler_Synthesizer), commit `5ca4cd8`, MIT | Pool di sample in PSRAM, carico selettivo dei preset, invii a chorus/delay/reverb, Freeverb con 4 comb e 3 allpass per canale, mix finale stereo. `limited()` limita con taglio netto a ±1; `fx_cubic_clipper.h` è un effetto di saturazione separato. Configurazione S3: 19 voci massime dichiarate con filtro canale, chorus e reverb. | Riutilizzare concetti di cache, acquisizione e send FX. Per VORTEX, SF2 è un motore opzionale futuro, non il nucleo dei quattro layer. Il clamp esistente evita overflow numerico ma può produrre distorsione udibile. |
| [Hackaday, 23 aprile 2026](https://hackaday.com/2026/04/23/esp32synth-an-audio-synthesis-library-for-the-esp32/) | Panoramica giornalistica di ESP32Synth e dei suoi benchmark; rinvia alla libreria. | Contesto, non prova delle prestazioni di VORTEX con granular, due display, quattro filtri e quattro uscite. |
| [ZmeyKolbasnik/Instruments](https://github.com/ZmeyKolbasnik/Instruments), commit `8c74bb8` | Collezione di SoundFont SF2 e file di descrizione, circa 376 MB nel clone e 79 SF2 nella radice; non contiene un motore DSP per ESP32. Il repository ha un file MIT, ma l'origine e i diritti dei singoli banchi non sono uniformemente dimostrati. | Usare solo come catalogo di timbri da valutare. Non copiare i banchi nel firmware, nel repository o in una distribuzione senza verificare la licenza di ciascuno. |

Il filtro di [N8 Synth](https://www.n8synth.co.uk/diy-eurorack/eurorack-ms-20-lowpass-filter/) resta il riferimento analogico: LM13700, due stadi OTA, retroazione con due LED e cutoff tramite corrente di controllo. Prevede alimentazione analogica bipolare e livelli Eurorack; l'ingresso PCM5102A deve essere tarato sul prototipo. Nessuno degli IC già posseduti sostituisce direttamente l'LM13700. La parte N8 non va assunta come limitatore dell'uscita master: i LED agiscono nella retroazione del filtro.

## Architettura raccomandata

```text
EK-128 / manopole / encoder -> Eventi e Parametri -> quattro LayerEngine
                                |                    DRUM, GRANULAR, SYNTH, NOISE/SF2 futuro
                                v
                  [L1 dry + reverb L1] -> peak guard L1 -> DAC A/L -> filtro 1 -- Level 1 --\
                  [L2 dry + reverb L2] -> peak guard L2 -> DAC A/R -> filtro 2 -- Level 2 ---+
                  [L3 dry + reverb L3] -> peak guard L3 -> DAC B/L -> filtro 3 -- Level 3 ---+-> mixer NE5532 -> master -> line/phones
                  [L4 dry + reverb L4] -> peak guard L4 -> DAC B/R -> filtro 4 -- Level 4 --/
```

Il filtro analogico di ogni layer resta dopo il proprio DAC, come deciso. Il primo mixer può essere **mono**: quattro attenuatori rotativi B10K, quattro resistenze d'ingresso e un sommatore invertente NE5532 con secondo stadio buffer/inversione; master rotativo dopo il sommatore, prima dell'uscita linea e dello stadio cuffie TDA1308. Usare alimentazione e disaccoppiamento analogici coerenti con il filtro. I valori delle resistenze vanno scelti misurando le uscite reali dei filtri, inclusa la massima risonanza; un rapporto iniziale conservativo `Rf/Rin ≈ 0,2` mantiene margine quando i quattro canali suonano insieme, ma non è uno schema definitivo. Aggiungere punti di test per DAC, filtro, sommatore e uscita.

```text
FILTRO 1 -> LEVEL 1 -> Rin 1 --\
FILTRO 2 -> LEVEL 2 -> Rin 2 ---+--> nodo (-) NE5532 A -- buffer NE5532 B -- MASTER -> LINE
FILTRO 3 -> LEVEL 3 -> Rin 3 ---+       |                                  \-> TDA1308 -> CUFFIE
FILTRO 4 -> LEVEL 4 -> Rin 4 --/        +-- Rf <-- uscita NE5532 A
                                        (+) a riferimento 0 V analogico
```

È uno schema funzionale per un prototipo, non un cablaggio componente-per-componente: verificare verso dei condensatori, carico dei potenziometri, stabilità e livelli del filtro prima di fissare valori/PCB.

Opzione stereo successiva: due sommatori L/R NE5532 e un potenziometro rotativo **doppio** per il pan di ciascun layer, oppure pan fissi. Servono più circuiti e quattro controlli aggiuntivi; il layer rimane mono fino al filtro. Gli eventuali effetti master digitali *dopo* i filtri richiedono ADC e un percorso di rientro dedicato. Per Rev.A gli FX digitali restano prima dei quattro DAC; `MASTER FX` nello schema concettuale esistente è una posizione futura, non già realizzata.

L'ESP32-S3 ha due periferiche I2S standard. Usarle con due DAC stereo è coerente con il target, ma sincronismo, deriva, ordine dei canali, disponibilità dei GPIO e DMA vanno verificati sul banco. I pin degli esempi esterni **confliggono** con i GPIO congelati della matrice: non copiarli. Fissare un unico sample rate (inizialmente 48 kHz) e blocchi da 64–128 frame; misurare glitch e sfasamento tra i due DAC, specialmente con transienti comuni.

## Struttura di codice fattibile

```text
audio/AudioGraph.{h,cpp}       renderizza quattro buffer mono per blocco
audio/I2SQuadOut.{h,cpp}       interleava L1/L2 e L3/L4 per due TX I2S
audio/PeakGuard.{h,cpp}        misura e limita i picchi prima di ogni DAC
engine/LayerEngine.{h,cpp}     stato e interfaccia comune delle quattro sorgenti
engine/Wavetable.{h,cpp}      oscillatore nativo, una voce poi polifonia misurata
engine/Granular.{h,cpp}       scheduler 4–8 grani iniziali, finestre Hann in LUT
fx/LongReverb.{h,cpp}         istanze mono o bus condivisi, buffer preallocati
storage/SampleCache.{h,cpp}   WAV in PSRAM, stream SD fuori dal callback audio
control/Matrix,Knobs,Encoder  scanner, filtro delle letture, eventi
control/ParameterStore       preset e mapping contestuale con soft takeover
ui/Displays                   sola lettura dello snapshot dei parametri
```

Contratto del renderer: per ogni blocco `render_layer(i, mono[i], frames)`, poi FX/limiter locali, poi `pack_i2s(mono[0], mono[1])` e `pack_i2s(mono[2], mono[3])`. Nessuna allocazione, I/O SD, aggiornamento display o lock bloccante nel callback audio. I comandi UI passano come eventi/snapshot applicati ai confini di blocco; coefficienti e curve delle manopole vengono calcolati nel task di controllo. Per partire dal codice esterno: **A)** adattare/forkare ESP32Synth per esporre i quattro bus prima della somma; **B, raccomandata per il primo prototipo quad**, scrivere un renderer VORTEX piccolo e importare solo componenti MIT realmente necessari dopo benchmark. L'API audio di SF2 richiede ancora più adattamenti perché termina in stereo e organizza i canali secondo il MIDI GM.

Budget di partenza, non prestazioni promesse: 48 kHz, quattro layer, primo granular con 4 grani, prime voci synth 4–8, un riverbero mono, poi aumentare separatamente. Un buffer mono di 2 s a 48 kHz/16 bit occupa circa 192 kB; 0,5 s cumulativi di delay `float` occupano circa 96 kB per istanza. Quattro istanze da 0,5 s richiederebbero circa 384 kB solo per i delay: la memoria può entrare in PSRAM, ma l'accesso e il costo CPU restano da misurare insieme a SD e display.

## Riverbero e filtri: tre opzioni reali

| Opzione | Audio | Controllo | Limite |
|---|---|---|---|
| R1 — quattro riverberi mono compatti | Ogni layer ha coda propria prima del proprio filtro. | Send e decay salvabili per layer. | CPU e traffico PSRAM massimi; provare 1→2→4. **Target finale preferito**. |
| R2 — due bus condivisi | Due coppie di layer inviano a due code, con ritorni assegnati a una o più uscite. | Send per layer. | Le code si mescolano tra le sorgenti; routing dei ritorni da definire. |
| R3 — un bus condiviso | Una sola coda globale con send per layer. | Semplice e leggero. | Non offre riverberi indipendenti; può essere solo la prima tappa di test. |

Per code molto lunghe usare un piccolo FDN o comb/allpass con damping e blocco DC; `RT60` circa 2–30 s è un **obiettivo di taratura**, non una misura ottenuta. `FREEZE` sospende l'ingresso e porta il feedback vicino a 1 con protezione di energia; uscita da freeze con rampa. Cambi di dimensione/delay richiedono interpolazione o cambio graduale per evitare click. La funzione `setTime()` del reverb SF2 cambia la lunghezza attiva dei delay, quindi non è da collegare direttamente a una manopola chiamata “decay” senza riprogettare la mappa del feedback.

MS-20: costruire e misurare un solo filtro N8 con il PCM5102A e un oscillatore a livello controllato, poi replicare. Il cutoff globale può sommarsi ai quattro CV locali tramite LM324 e reti di resistenze, ma la scala effettiva va tarata sul convertitore esponenziale. La risonanza globale meccanica con potenziometro a quattro sezioni è possibile; un controllo digitale del feedback è una Rev.B più complessa. Un filtro digitale MS-20-like rimane opzionale come pre-filtro, senza sostituire i quattro analogici.

## Distorsione: dove prevenirla

1. **Dentro il DSP:** accumulatori con margine, guadagni per layer e FX normalizzati; un misuratore del picco pre-DAC per canale. In V1 un limiter di picco con attacco rapido, rilascio graduale e ceiling iniziale ~−3 dBFS, più saturazione finale lieve solo come rete di sicurezza. Un vero peak limiter può usare 1–2 ms di lookahead; verificarne latenza e CPU. Il semplice clamp `±1` del progetto SF2 produce clipping duro.
2. **Nel filtro:** tarare il livello dal PCM5102A e il drive; la risonanza MS-20 può generare armoniche intenzionali e picchi maggiori dell'ingresso. Il limiter pre-DAC non controlla ciò che il filtro crea in seguito.
3. **Nel mixer:** rapporto di somma conservativo, ±12 V analogici come nel filtro di riferimento, headroom verificato con quattro segnali e massima risonanza, indicatore di clip sul master (rilevatore di picco/comparatore LM393). Un attenuatore/master non ripara una saturazione già avvenuta nel sommatore.
4. **Se serve una soglia analogica automatica reale:** progettare in una revisione successiva un rivelatore d'inviluppo + VCA con un **LM13700 aggiuntivo**, prima dello stadio d'uscita, e provarne attacco/rilascio e distorsione. Diodi di clamp all'uscita possono proteggere da picchi estremi ma introducono essi stessi distorsione: non chiamarli “limiter trasparente”.

Obiettivo di collaudo: nessun sample clip digitale nel normale preset massimo; mixer senza clipping visibile all'oscilloscopio con quattro layer attivi; test separato di filtro a risonanza estrema, dove la saturazione musicale resta selezionabile.

## Manopole e tasti EK-128

Pannello base proposto: **16 potenziometri rotativi B10K** in quattro gruppi da quattro: `LEVEL L1–4`, `REVERB SEND L1–4`, `CUTOFF L1–4`, `RESONANCE L1–4`. `MASTER` e `GLOBAL CUTOFF` sono due manopole dedicate aggiuntive: **18 in totale**. `GLOBAL RESONANCE` è un'opzione successiva, preferibilmente meccanica a quattro sezioni e da testare. I potenziometri cutoff/risonanza possono agire direttamente sui circuiti analogici; leggere una copia ad alta impedenza per la GUI solo se serve, senza caricare il nodo CV/audio.

Per i potenziometri letti dal firmware: un CD74HC4067 analogico può leggere fino a 16 manopole da un ADC, condividendo S0–S3 con lo scanner solo in finestre temporali distinte. I LEVEL nel mixer e i controlli diretti del filtro non hanno bisogno dell'ADC per funzionare; se si desidera mostrarli/salvarli, la lettura deve essere separata elettricamente dal percorso audio/CV. La lettura non deve essere eseguita mentre si acquisiscono le righe tasti; dopo ogni commutazione lasciare assestare la tensione e scartare eventualmente la prima conversione. Usare ADC interno per primo test, poi confrontare rumore/linearità con MCP3008 a 3,3 V se **si verifica che il chip è posseduto** e che SPI/pin restano disponibili. Gli altri quattro 4067 riferiti dall'utente non richiedono un acquisto immediato.

Per parametri contestuali sono utili **1–2 encoder rotativi senza fine** con pressione. Un encoder non è un potenziometro: emette incrementi/decrementi. Consente di editare decay, grain size, density, spray, posizione, velocità, FX feedback o preset senza il salto di valore di una manopola assoluta. Per i potenziometri che cambiano funzione con la pagina, usare *soft takeover*: finché la posizione fisica non raggiunge il valore salvato, non cambiare il parametro. Rallentare i passi dell'encoder premuto e accelerarli quando ruota veloce; mostrare sempre il valore sui display.

Mappa di interazione compatibile con la [keymap V1.0](05_KEYMAP_V1_0.md):

| Tasto EK-128 | Gesto proposto | Encoder / potenziometro |
|---|---|---|
| `A1–A4` | Selezione layer, senza interrompere gli altri. | Encoder modifica il parametro della pagina del layer; le manopole dedicate continuano ad agire sui rispettivi layer. |
| `A10 GRANULAR` | Apre la pagina granular; `D5 CAPTURE` avvia/termina una cattura, `D1 FREEZE` congela il buffer, `D2 HOLD` mantiene il gate. | Encoder 1: posizione; encoder 2: grain length. Premendo l'encoder: passi fini o scelta sorgente. `D13/D14` restano scorciatoie density. |
| `E1–E5` | Selezione della finestra del grano; `E12` reverse %. | Encoder modifica spray/chaos/reverse con parametro evidenziato; la manopola `REVERB SEND` del layer resta sempre separata. |
| `G1–G14` | Selezione effetto; seconda pressione abilita/disabilita, pressione mantenuta offre effetto momentaneo solo dove musicalmente sensato. | Encoder 1: parametro principale (p.es. tempo delay o decay reverb); encoder 2: feedback/damping. `G15` freeze FX, `G16` kill con rampa per evitare click. |
| `A15 MIX`, `A5 MUTE`, `A6 SOLO` | Mixer su display, mute/solo del layer selezionato. | Quattro LEVEL fisici agiscono sempre; encoder modifica l'eventuale pan/route digitale o valore fine, non sostituisce il livello analogico. |

Per cattura, resample, bypass e freeze distinguere `key down`, `key up`, pressione lunga e stato persistente; un indicatore LED/display deve dire chiaramente quando si sta registrando. I gesti sono una proposta di firmware, da provare sulla tastiera prima di aggiornare la keymap congelata.

Un mute eseguito solo prima del DAC potrebbe lasciare udibile un filtro in auto-oscillazione. Il primo firmware può silenziare le sorgenti digitali; per un MUTE/SOLO che azzera davvero l'uscita del layer servirà un controllo di guadagno o interruttore analogico **dopo** il filtro, da scegliere e provare insieme al mixer. Non usare il CD74HC4067 a 3,3 V direttamente su un segnale audio bipolare ±12 V.

## Gate concreti prima della PCB

1. Confermare scanner C0/C1, poi 128 tasti; misurare falsi eventi durante scansione dei potenziometri.
2. DAC A con due toni indipendenti, poi DAC B; verificare sincronismo, livelli e assenza di glitch con entrambe le periferiche I2S.
3. Primo filtro analogico: livelli, cutoff, risonanza, rumore e comportamento a caldo.
4. Mixer NE5532 mono a quattro ingressi con carico fittizio e prova a quattro segnali; misurare headroom e clip LED. Poi stadio linea/cuffie.
5. Renderer quad asciutto, WAV/PSRAM, granular 4 grani; misurare CPU per blocco, DMA underrun, RAM interna e PSRAM.
6. Un riverbero e limiter per canale; misurare RT60, coda in freeze, eventuali click e picchi. Solo dopo provare 2 e 4 riverberi con i due display attivi.

### Riferimenti tecnici primari

- [Espressif: I2S ESP32-S3](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/i2s.html): due periferiche, standard stereo L/R.
- [TI: PCM5102A](https://www.ti.com/product/PCM5102A): uscita di linea nominale 2,1 Vrms a scala piena.
- [TI: NE5532](https://www.ti.com/product/NE5532): alimentazione e caratteristiche del sommatore analogico.
- [Microchip: MCP3008](https://www.microchip.com/en-us/product/MCP3008): ADC SPI 10 bit, otto canali.
- [N8 Synth: filtro MS-20](https://www.n8synth.co.uk/diy-eurorack/eurorack-ms-20-lowpass-filter/): topologia, BOM e taratura.
