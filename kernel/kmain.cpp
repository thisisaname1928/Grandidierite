#include <cstdint>

uint16_t *buffer = (uint16_t *)0xb8000;
uint16_t x = 0, y = 0;
uint8_t col = 0xbf;

void putc(char c) {
  buffer[x * 25 + 80] = c & (col << 8);

  x++;
}

void print(const char *s) {
  int i = 0;
  while (s[i] != 0) {
    putc(s[i]);
    i++;
  }
}

extern "C" void kmain() { print("HIII"); }