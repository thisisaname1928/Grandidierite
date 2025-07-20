#include "arch/amd64/amd64.hpp"
#include "arch/arch.hpp"
#include <cstdint>

uint16_t *buffer = (uint16_t *)0xb8000;
uint16_t x = 0, y = 0;
uint8_t col = 0x0f;

void putc(char c) {
  buffer[y * 80 + x] = c | (col << 8);

  x++;
}

void print(const char *s) {
  int i = 0;
  while (s[i] != 0) {
    putc(s[i]);
    i++;
  }
}

extern "C" void kmain() {
  print("jsjsjsskks just boot into kernel!!!!");

  Amd64 archAMD64;
  arch = &archAMD64;

  arch->out(0x3F8, (uint8_t)'A');

  for (;;) {
  }
}
