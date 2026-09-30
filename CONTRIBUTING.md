# VORTEX AX-128 Working Rules

## One prototype step per commit

Keep each commit limited to one clear outcome, for example:

- `docs: import VORTEX AX-128 project baseline`
- `feat: add modular CD74HC4067 matrix test`
- `test: record successful A1-A16 mux scan`
- `fix: correct Row C pin mapping`

Do not mix a pinout change, a DSP experiment and asset cleanup in the same commit.

## Hardware evidence

For each physical test, create:

```text
assets/test_evidence/YYYY-MM-DD/step-name/
```

Store only useful evidence:

- one wiring overview photo;
- close-ups needed to verify connections;
- Serial Monitor output or screenshot;
- optional short notes in `RESULT.md`.

Before committing a phone photo, remove GPS/location metadata. Never commit Wi-Fi credentials, tokens, private keys or unrelated personal information visible on screen.

## Firmware status

Firmware filenames or local README files must state one of:

- `TESTED` — compiled and confirmed on the physical prototype;
- `COMPILED` — builds successfully but has not been bench-tested;
- `UNTESTED` — design draft only.

After a successful bench test, update:

- `docs/12_TEST_LOG_AND_MILESTONES.md`;
- `docs/13_NEXT_STEPS.md`;
- `PROJECT_MANIFEST.json`.

If a frozen decision changes, also update `docs/02_FROZEN_DECISIONS.md` in the same commit.

## External DSP code

Do not copy code or audio assets into the firmware until repository, dependency and asset licenses have been reviewed. Projects with GPL or no explicit license remain study references unless the licensing strategy is deliberately changed and documented.

