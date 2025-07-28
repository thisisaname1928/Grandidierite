#include "../boot/adachiite.h"
#include "arch/amd64/GDT/GDT.hpp"
#include "arch/amd64/amd64.hpp"
#include "arch/arch.hpp"
#include "driver/driver.hpp"
#include "driver/framebuffer/GOP/GOP.hpp"
#include "kprintf/kprintf.hpp"
#include <cstddef>
#include <cstdint>

AdachiiteBootInfo *adachiiteBootInfo;

extern "C" void kmain(AdachiiteBootInfo *info) {
  adachiiteBootInfo = info;

  Amd64 archAMD64;
  archAMD64.init();
  AMD64::initGDT();
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

  MemMapDesc *memmap = (MemMapDesc *)adachiiteBootInfo->memoryMap;
  uint64_t curSize = 0;
  uint64_t freeMemLeft = 0;
  while (curSize < info->memoryMapSize) {
    if (memmap->Type == USABLE)
      freeMemLeft += memmap->NumberOfPages * 0x1000;

    memmap = (MemMapDesc *)((char *)memmap + info->memoryDescSize);
    curSize += info->memoryDescSize;
  }

  kprintf("free mem: %u MB\n", (uint64_t)freeMemLeft / 1024 / 1024);

  archAMD64.markPage((uint64_t)kmain, true);
  archAMD64.markPage((uint64_t)kmain + 4, false);
  uint64_t s = archAMD64.checkPageStatus((uint64_t)kmain);
  kprintf("check mapped memory %u\n", s);

  for (;;) {
  }
}
