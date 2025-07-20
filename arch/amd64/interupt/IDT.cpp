#include "IDT.hpp"
#include "kernel/kprintf/kprintf.hpp"
#include <cstdint>

extern "C" void IDTTest() {
  kprintf("UAOOOOOOOOO\n");
  for (;;) {
  }
}

extern "C" void loadIDT(uint64_t);

namespace AMD64 {

// align for faster access
IDT64 __attribute__((aligned(0x1000))) kernelIDTTab[256];
IDTR64 kernelIDTR = {.size = sizeof(kernelIDTTab),
                     .offset = (uint64_t)&kernelIDTTab};

IDT64 createEntry(void *offset, uint16_t codeSegment, uint8_t IST,
                  uint8_t attribute) {
  uint64_t address = (uint64_t)offset;

  IDT64 tab;

  // chop the offset
  tab.offsetLow = address & 0xffff;
  tab.offsetMid = (address >> 16) & 0xffff;
  tab.offsetHigh = (address >> 32) & 0xffffffff;

  tab.CS = codeSegment;
  tab.IST = IST;
  tab.attribute = attribute; // note: if there is a entry that not present, cpu
                             // will not accept all of the table

  return tab;
}

void initKernelIDTTab() {
  for (int i = 0; i < 256; i++) {
    kernelIDTTab[i] =
        createEntry((void *)IDTTest, 0x8, 0, PRESENT | RING_0 | TRAP_GATE);
  }

  loadIDT((uint64_t)&kernelIDTR);
}

} // namespace AMD64
