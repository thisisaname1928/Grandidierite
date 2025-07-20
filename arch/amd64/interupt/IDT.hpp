#pragma once

#include <cstdint>

namespace IDT64 {

#define INTERRUPT_GATE 0xe
#define TRAP_GATE 0xf
#define TASK_GATE 0x5
#define RING_0 0
#define RING_1 (1 << 4)
#define RING_2 (2 << 4)
#define RING_3 (3 << 4)
#define PRESENT (1 << 7)

typedef struct __attribute__((packed)) {
  uint16_t size;
  uint64_t offset;
} IDTR64;

typedef struct __attribute__((packed)) {
  uint16_t offsetLow;
  uint16_t CS;
  uint8_t IST;
  uint8_t attribute;
  uint16_t offsetMid;
  uint32_t offsetHigh;
  uint32_t reserved;
} IDT64;

extern IDT64 __attribute__((aligned(0x1000))) kernelIDTTab[256];
IDT64 createEntry(void *offset, uint16_t codeSegment, uint8_t IST,
                  uint8_t attribute);
void initKernelIDTTab();

} // namespace IDT64