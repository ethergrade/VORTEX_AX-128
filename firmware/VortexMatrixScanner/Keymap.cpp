#include "Keymap.h"

#include "VortexConfig.h"

namespace vortex {
namespace {

constexpr const char* kKeyNames[config::kMaxRowCount][config::kMaxColumnCount] = {
    {"LAYER 1", "LAYER 2", "LAYER 3", "LAYER 4", "MUTE", "SOLO", "COPY", "PASTE",
     "DRUM", "GRANULAR", "SYNTH", "ANALOG", "FX", "SEQ", "MIX", "SYSTEM"},
    {"STEP 1", "STEP 2", "STEP 3", "STEP 4", "STEP 5", "STEP 6", "STEP 7", "STEP 8",
     "STEP 9", "STEP 10", "STEP 11", "STEP 12", "STEP 13", "STEP 14", "STEP 15", "STEP 16"},
    {"BD", "BD2", "SNARE", "CLAP", "RIM", "LT", "MT", "HT", "CLOSED HH", "OPEN HH",
     "CRASH", "RIDE", "COWBELL", "PERC 1", "PERC 2", "USER DRUM"},
    {"FREEZE", "HOLD/GATE", "RETRIGGER", "REVERSE", "CAPTURE", "RESAMPLE", "SYNC/FREE", "RANDOMIZE",
     "POSITION -", "POSITION +", "LENGTH -", "LENGTH +", "DENSITY -", "DENSITY +", "PITCH -", "PITCH +"},
    {"ENV SQUARE", "ENV TRAPEZOID", "ENV HANN", "ENV GAUSSIAN", "ENV TRIANGLE", "SPRAY -", "SPRAY +", "CHAOS -",
     "CHAOS +", "SPACE -", "SPACE +", "REVERSE %", "SOURCE -", "SOURCE +", "BANK -", "BANK +"},
    {"NOTE 1", "NOTE 2", "NOTE 3", "NOTE 4", "NOTE 5", "NOTE 6", "NOTE 7", "NOTE 8",
     "NOTE 9", "NOTE 10", "NOTE 11", "NOTE 12", "NOTE 13", "NOTE 14", "NOTE 15", "NOTE 16"},
    {"CHORUS", "FLANGER", "PHASER", "TREMOLO", "VIBRATO", "DELAY", "REVERB", "DRIVE",
     "BITCRUSH", "WAVEFOLD", "STUTTER", "TAPE STOP", "REVERSE FX", "GLITCH", "FX FREEZE", "FX BYPASS/KILL"},
    {"PLAY/STOP", "RECORD", "OVERDUB", "TAP TEMPO", "TEMPO -", "TEMPO +", "PATTERN -", "PATTERN +",
     "SCENE 1", "SCENE 2", "SCENE 3", "SCENE 4", "SCENE 5", "SCENE 6", "SCENE 7", "SCENE 8"},
};

}  // namespace

const char* keyName(uint8_t row, uint8_t column) {
  if (row >= config::kMaxRowCount || column >= config::kMaxColumnCount) {
    return "UNKNOWN";
  }

  return kKeyNames[row][column];
}

}  // namespace vortex

