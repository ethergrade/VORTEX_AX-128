#pragma once

#include <Arduino.h>

namespace vortex::config {

inline constexpr uint8_t kMaxRowCount = 8;
inline constexpr uint8_t kMaxColumnCount = 16;

inline constexpr uint8_t kMuxSelectPins[4] = {4, 6, 7, 15};
inline constexpr uint8_t kRowPins[kMaxRowCount] = {5, 16, 17, 18, 8, 9, 10, 11};

inline constexpr uint8_t kActiveRowCount = 8;
inline constexpr uint8_t kActiveColumnCount = 16;

inline constexpr uint16_t kDebounceMillis = 20;
inline constexpr uint16_t kMuxSettleTimeMicros = 10;
inline constexpr uint16_t kLoopDelayMillis = 1;

static_assert(kActiveRowCount <= kMaxRowCount);
static_assert(kActiveColumnCount <= kMaxColumnCount);
static_assert(kDebounceMillis > 0);

}  // namespace vortex::config
