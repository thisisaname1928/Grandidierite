#include "../boot/adachiite.h"
#include "arch/amd64/amd64.hpp"
#include "arch/arch.hpp"
#include "driver/driver.hpp"
#include "driver/framebuffer/GOP/GOP.hpp"
#include "kprintf/kprintf.hpp"
#include "mm/pageAllocation.hpp"
#include <cstddef>
#include <cstdint>
#include <stdint.h>
AdachiiteBootInfo *adachiiteBootInfo;

extern "C" void kmain(AdachiiteBootInfo *info) {
  adachiiteBootInfo = info;

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

  int s = archAMD64.mapPage(0x600000, KERNEL_VIRTUAL_ADDRESS + 0x1000000);

  if (!s)
    kprintf("map page failed\n");
  uint8_t *ptr1 = (uint8_t *)0x600000;
  *ptr1 = '&';
  uint8_t *ptr = (uint8_t *)(KERNEL_VIRTUAL_ADDRESS + 0x1000000);

  kprintf("look what i got: %c %c", *ptr, *ptr1);

  for (;;) {
  }
}
