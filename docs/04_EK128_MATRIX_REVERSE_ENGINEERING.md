# 04 — EK-128 Matrix Reverse Engineering

## Coordinate convention

128 keys are referenced as:

- rows `A` through `H`
- columns `1` through `16`

Example:

```text
A1 ... A16
B1 ... B16
...
H1 ... H16
```

## Confirmed electrical structure

The EK-128 PCB contains per-key diodes.

This is advantageous because it reduces ghosting and permits overlapping key presses.

## Confirmed rows

Measured with multimeter diode mode:

```text
A = J1-3
B = J1-5
C = J1-7
D = J1-9
E = J1-11
F = J1-13
G = J1-15
H = J1-17
```

Measured diode drop is approximately 0.54–0.55 V.

## Confirmed columns

```text
1  = J1-2
2  = J1-4
3  = J1-6
4  = J1-8
5  = J1-10
6  = J1-12
7  = J1-14
8  = J1-16
9  = J1-18
10 = J1-20
11 = J1-22
12 = J1-24
13 = J1-26
14 = J1-28
15 = J1-30
16 = J1-32
```

Spot checks include:

- A1 -> J1-2 + Row A/J1-3
- A2 -> J1-4 + Row A/J1-3
- A15 -> J1-30 + Row A/J1-3
- A16 -> J1-32 + Row A/J1-3
- B1 -> J1-2 + J1-5
- C1 -> J1-2 + J1-7
- D1 -> J1-2 + J1-9
- E1 -> J1-2 + J1-11
- F1 -> J1-2 + J1-13
- G1 -> J1-2 + J1-15
- H1 -> J1-2 + J1-17

## Diode direction / scan consequence

With the measured orientation, the successful scan method is:

- rows = `INPUT_PULLUP`;
- active column = pulled LOW;
- nonselected columns = disconnected/high impedance;
- pressed key pulls its row LOW through the diode.

The CD74HC4067 is a good fit because only the selected column is connected to `SIG`.

## First live test

Test wiring:

```text
J1-2 -> 1 kΩ -> GPIO4
J1-3 ---------> GPIO5 INPUT_PULLUP
```

Result:

```text
A1 PRESSED
A1 RELEASED
```

worked correctly in Serial Monitor.

## Two-key scan test

Test wiring:

```text
J1-2 -> GPIO4
J1-4 -> GPIO6
J1-3 -> GPIO5
```

Software alternated the two columns, driving only one LOW at a time.

A1 and A2 were correctly distinguished.

Overlapping presses also worked correctly:

- hold A1;
- press A2;
- release A2;
- A1 remains held;
- press A2 again;
- release A1 while A2 stays held;
- release A2.

This validated the matrix scan strategy.
