#include "../abstractLayer.hpp"
#pragma once

class Amd64 : public ArchitectureAbstractLayer {
public:
  ~Amd64();
  void out(uint16_t port, uint8_t value) override;
  void out(uint16_t port, uint16_t value) override;
  void out(uint16_t port, uint32_t value) override;
  uint8_t inb(uint16_t port) override;
  uint16_t inw(uint16_t port) override;
  uint32_t ind(uint16_t port) override;
  void init() override;
  void setInterrupt(int n, int type, int dpl, void *offset) override;
};