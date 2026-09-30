# 09 — Power and Audio I/O

## Current power concept

The ESP32 logic is 3.3 V, but the entire instrument does not need to run at one voltage.

Target internal rails:

```text
+3.3 V -> ESP32 logic, mux logic, SD logic
+5 V   -> suitable audio modules / headphone amp
+12 V  -> analog rails if required
-12 V  -> analog filter/op-amp rails if required
GND    -> common reference, with careful analog/digital layout
```

Use an external certified DC power source. Do not expose/build mains 230 V circuitry inside VORTEX.

## TDA1308 headphone amplifier

Chosen as dedicated stereo headphone stage.

Target:

```text
PCM / MASTER LINE
      │
      ├── LINE OUT L/R
      │
      └── TDA1308 -> HEADPHONE VOL -> 3.5 mm PHONES
```

TDA1308 supply target: 5 V.

## PCM5102A

Chosen I2S DAC family.

Prototype direction:

- two PCM5102A modules;
- four independent mono layer outputs;
- 44.1 or 48 kHz;
- line-level signal.

## Grounding

All low-voltage subsystems share a common electrical reference, but the final PCB should be laid out carefully to reduce digital noise coupling into analog audio.

Keep:

- USB/ESP32/display switching currents away from analog input nodes;
- analog decoupling local;
- short audio paths;
- appropriate power filtering;
- clear test points.
