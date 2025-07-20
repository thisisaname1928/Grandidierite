#include <cstdint>
#pragma once

// pls dont pass more than 6 args...
extern "C" void kprintf(const char *str, ...);

void print(const char *str);
void printUint(uint64_t n);
void printHex(uint64_t n);
void printBin(uint64_t n);
void printInt8(int8_t n);
void printInt16(int16_t n);
void printInt32(int32_t n);
void printInt64(int64_t n);