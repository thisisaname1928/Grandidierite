#pragma once

#include <cstdint>
class ArchitectureAbstractLayer {
public:
  virtual void out(uint16_t port, uint8_t value) = 0;
  virtual void out(uint16_t port, uint16_t value) = 0;
  virtual void out(uint16_t port, uint32_t value) = 0;
  virtual uint8_t inb(uint16_t port) = 0;
  virtual uint16_t inw(uint16_t port) = 0;
  virtual uint32_t ind(uint16_t port) = 0;
  virtual void init() = 0;
};