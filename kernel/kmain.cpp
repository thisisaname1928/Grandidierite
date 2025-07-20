#include "arch/amd64/amd64.hpp"
#include "arch/arch.hpp"
#include "driver/driver.hpp"
#include "kprintf/kprintf.hpp"
#include <cstdint>

uint16_t *buffer = (uint16_t *)0xb8000;
uint16_t x = 0, y = 0;
uint8_t col = 0x0f;

void putc(char c) {
  buffer[y * 80 + x] = c | (col << 8);

  x++;
}

void printv(const char *s) {
  int i = 0;
  while (s[i] != 0) {
    putc(s[i]);
    i++;
  }
}

extern "C" void kmain() {
  printv("jsjsjsskks just boot into kernel!!!!");

  Amd64 archAMD64;
  arch = &archAMD64;

  initDriver();

  print("HI I AM KERNEL\n");
  kprintf("... %b %c %s %ib %iw", (uint64_t)0xffff, 'c', "gudd", (uint8_t)-12,
          (uint16_t)-123);

  for (;;) {
  }
}
