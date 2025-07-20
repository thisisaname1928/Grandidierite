#include "driver/driver.hpp"
#include <cstdint>

void print(const char *str) {
  while (*str != 0) {
    debugDevice->putc(*str);
    str++;
  }
}

void printUint(uint64_t n) {
  if (n == 0) {
    print("0");
    return;
  }

  char buff[20];
  buff[19] = 0;
  int c = 18;

  while (n != 0) {
    buff[c] = n % 10 + 48;
    c--;

    n /= 10;
  }

  print(&buff[c + 1]);
}

char hexTransTab[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void printHex(uint64_t n) {
  print("0x");
  if (n == 0) {
    print(0);
    return;
  }

  char buff[20];
  buff[19] = 0;
  int c = 18;

  while (n != 0) {
    buff[c] = hexTransTab[n % 16];
    c--;

    n /= 16;
  }

  print(&buff[c + 1]);
}

void printBin(uint64_t n) {
  print("0b");
  if (n == 0) {
    print(0);
    return;
  }

  char buff[66];
  buff[65] = 0;
  int c = 64;

  while (n != 0) {
    buff[c] = n % 2 + 48;
    c--;

    n /= 2;
  }

  print(&buff[c + 1]);
}

// print func for every int size, these functions will be used by kprintf so we
// need to divide them
#define PRINT_INT(bit, bufLength)                                              \
  void printInt##bit(int##bit##_t n) {                                         \
    if (n == 0) {                                                              \
      print("0");                                                              \
      return;                                                                  \
    }                                                                          \
                                                                               \
    if (n < 0) {                                                               \
      print("-");                                                              \
      n *= -1;                                                                 \
    }                                                                          \
                                                                               \
    char buff[bufLength];                                                      \
    buff[bufLength - 1] = 0;                                                   \
    int c = bufLength - 2;                                                     \
                                                                               \
    while (n != 0) {                                                           \
      buff[c] = n % 10 + 48;                                                   \
      c--;                                                                     \
                                                                               \
      n /= 10;                                                                 \
    }                                                                          \
                                                                               \
    print(&buff[c + 1]);                                                       \
  }

PRINT_INT(8, 5);
PRINT_INT(16, 7);
PRINT_INT(32, 20);
PRINT_INT(64, 20);

extern "C" uint64_t args;
extern "C" void kprintfReal() {
  uint64_t *arguments = (uint64_t *)&args;

  const char *str = (const char *)arguments[0];
  int count = 1;
  for (uint32_t i = 0; str[i] != 0; i++) {
    if (str[i] == '%') {
      if (str[i + 1] == 'c') {
        char c = (char)arguments[count];
        count++;
        debugDevice->putc(c);
        i++;
        continue;
      } else if (str[i + 1] == 's') {
        char *c = (char *)arguments[count];
        count++;

        print(c);
        i++;
        continue;
      } else if (str[i + 1] == 'u') {
        uint64_t a = arguments[count];
        count++;

        printUint(a);
        i++;
        continue;
      } else if (str[i + 1] == 'x') {
        uint64_t a = arguments[count];
        count++;

        printHex(a);
        i++;
        continue;
      } else if (str[i + 1] == 'b') {
        uint64_t a = arguments[count];
        count++;

        printBin(a);
        i++;
        continue;
      } else if (str[i + 1] == 'i') { // integer
        if (str[i + 2] == 'b') {
          int8_t a = arguments[count];
          count++;

          printInt8(a);
          i += 2;
          continue;
        } else if (str[i + 2] == 'w') {
          int16_t a = arguments[count];
          count++;

          printInt16(a);
          i += 2;
          continue;
        } else if (str[i + 2] == 'd') {
          int32_t a = arguments[count];
          count++;

          printInt32(a);
          i += 2;
          continue;
        } else if (str[i + 2] == 'q') {
          int64_t a = arguments[count];
          count++;

          printInt64(a);
          i++;
          continue;
        }
      }
    }

    debugDevice->putc(str[i]);
  }
}