#pragma once

#include <cstdint>

namespace Gradidierite {
enum INTERRUPT_TYPE { INTERRUPT_EXECEPTION = 0x12 };
} // namespace Gradidierite

class ArchitectureAbstractLayer {
public:
  virtual void out(uint16_t port, uint8_t value) = 0;
  virtual void out(uint16_t port, uint16_t value) = 0;
  virtual void out(uint16_t port, uint32_t value) = 0;
  virtual uint8_t inb(uint16_t port) = 0;
  virtual uint16_t inw(uint16_t port) = 0;
  virtual uint32_t ind(uint16_t port) = 0;
  virtual void init() = 0;
  virtual void setInterrupt(int n, int type, int dpl, void *offset) = 0;
};