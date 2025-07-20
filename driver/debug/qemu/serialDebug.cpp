#include "serialDebug.hpp"
#include "arch/arch.hpp"
#include <cstdint>

void QemuSerialDebug::putc(char c) { arch->out(0x3f8, (uint8_t)c); }

const char *QemuSerialDebug::getName() { return "QEMU_SERIAL_DEBUG"; }