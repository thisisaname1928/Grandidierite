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
      GOP.putPixel(0xdeadbeef, x, y);

  if (info == NULL) {
    kprintf("Grandidierite need Adachiite bootloader!\n");
  }

  kprintf("framebuffer: %ux%u\n", info->fb.horizontalResolution,
          info->fb.verticalResolution);

  print("HI I AM KERNEL\n");
  kprintf("... %b %c %s %ib %iw", (uint64_t)0xffff, 'c', "gudd", (uint8_t)-12,
          (uint16_t)-123);

  for (;;) {
  }
}
