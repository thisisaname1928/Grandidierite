#include "../boot/adachiite.h"
#include "arch/amd64/amd64.hpp"
#include "arch/arch.hpp"
#include "driver/driver.hpp"
#include "driver/framebuffer/GOP/GOP.hpp"
#include "kprintf/kprintf.hpp"
#include "mm/pageAllocation.hpp"
#include <cstddef>
#include <stdint.h>
AdachiiteBootInfo *adachiiteBootInfo;
// make a copy so paging stuffs wont affect it
AdachiiteBootInfo bootInfo;

extern "C" void kmain(AdachiiteBootInfo *info) {
  bootInfo = *info;
  adachiiteBootInfo = &bootInfo;

  Amd64 archAMD64;
  archAMD64.init();
  arch = &archAMD64;
  GOPFramebufferDriver GOP;
  GOP.init();

  initDriver();

  for (int x = 0; x < 255; x++)
    for (int y = 0; y < 255; y++)
      GOP.putPixel(0x545e7a, x, y);

  if (info == NULL) {
    kprintf("Grandidierite need Adachiite bootloader!\n");
  }

  MemoryManagement::pageAllocationInit();
  uint64_t a = 129;
  MemoryManagement::allocPages(&a);

  for (;;) {
  }
}
