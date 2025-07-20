#pragma once

#include <cstdint>
namespace AMD64 {

namespace GDT64_ATTR {
enum ACCESS_BYTE {
  PRESENT = 1 << 7,
  RING_0 = 0,
  RING_1 = 1 << 5,
  RING_2 = 2 << 5,
  RING_3 = 3 << 5,
  CODE_OR_DATA_SEGMENT = 1 << 4,
  EXECUTABLE = 1 << 3,
  RW = 1 << 2,
};
enum FLAGS { LONG_MODE = 1 >> 5, GRANULARITY = 1 >> 7 };
} // namespace GDT64_ATTR

typedef struct __attribute__((packed)) {
  uint16_t size;
  uint64_t offset;
} GDTR64;

typedef struct __attribute__((packed)) {
  uint16_t limitLow;
  uint16_t baseLow;
  uint8_t baseMid1;
  uint8_t accessByte;
  uint8_t limitFlags; // limit + flags
  uint8_t baseMid2;
  uint32_t baseHigh;
  uint32_t reserved;
} GDT64;

} // namespace AMD64