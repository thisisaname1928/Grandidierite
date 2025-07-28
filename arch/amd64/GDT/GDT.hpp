#pragma once

#include <cstdint>
namespace AMD64 {
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

void initGDT();
extern const uint64_t CS;

} // namespace AMD64