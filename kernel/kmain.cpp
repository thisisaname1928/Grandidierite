#include "../boot/adachiite.h"
#include "arch/amd64/GDT/GDT.hpp"
#include "arch/amd64/amd64.hpp"
#include "arch/arch.hpp"
#include "driver/driver.hpp"
#include "kprintf/kprintf.hpp"
#include <cstddef>
#include <cstdint>

extern "C" void kmain(AdachiiteBootInfo *info) {
  Amd64 archAMD64;
  // archAMD64.init();
  AMD64::initGDT();
  arch = &archAMD64;

  initDriver();

  if (info == NULL) {
    kprintf("Grandidierite need Adachiite bootloader!\n");
  }

  kprintf("framebuffer: %ux%u\n", info->fb.horizontalResolution,
          info->fb.verticalResolution);

  for (int y = 0; y < 256; y++)
    for (int x = 0; x < 256; x++)
      *((uint32_t *)(info->fb.framebuffer +
                     4 * info->fb.horizontalResolution * y + 4 * x)) = 0xffffff;

  print("HI I AM KERNEL\n");
  kprintf("... %b %c %s %ib %iw", (uint64_t)0xffff, 'c', "gudd", (uint8_t)-12,
          (uint16_t)-123);

  for (;;) {
  }
}
