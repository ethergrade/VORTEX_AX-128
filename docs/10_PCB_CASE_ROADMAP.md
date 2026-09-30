# 10 — PCB and Case Roadmap

## Final PCB objective

Once the complete bench prototype is validated, create a custom VORTEX AX-128 mainboard in KiCad.

The final design should consolidate:

- EK-128 J1 interface;
- ESP32-S3 section/module;
- keyboard CD74HC4067;
- pot mux;
- microSD;
- two PCM5102A-equivalent DAC channels/modules or integrated DAC implementation;
- four analog filter channels;
- analog mixer;
- TDA1308 headphone stage;
- line outputs;
- two display connectors;
- USB/programming;
- 3.3 V / 5 V / ±12 V power sections;
- test points;
- expansion headers;
- control connectors.

## PCB deliverables

- `.kicad_pro`
- schematic
- PCB
- Gerber
- NC drill
- BOM
- CPL/position
- fabrication notes
- assembly notes

## Routing / layout priorities

- separate noisy digital zone from sensitive analog zone;
- local bypass capacitors at every IC;
- adequate power trace widths;
- analog ground strategy;
- short filter feedback paths;
- controlled routing of I2S/SPI;
- test pads;
- clearly labelled connectors;
- modular sections where practical.

## Case CAD

Preferred software direction:

- Shapr3D on Mac

Possible alternatives:

- Onshape
- Fusion
- FreeCAD

Target exports:

- STEP
- 3MF
- STL

## Case concept

Retro workstation / sampler appearance.

Must accommodate:

- EK-128;
- two identical displays;
- 16 potentiometer positions;
- line outputs;
- headphone jack;
- headphone volume;
- USB-C;
- microSD;
- ventilation where required;
- PCB standoffs;
- serviceability.

Target construction:

- base + top;
- ~2.5–3 mm walls as starting point;
- heat-set threaded inserts for final design;
- dimensional prototype before final print.
