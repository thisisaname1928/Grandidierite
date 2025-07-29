#include "../abstractLayer.hpp"
#include <cstdint>
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
  bool checkMappingAddress(uint64_t virtualAddress) override;
  bool checkPageStatus(uint64_t virtualAddress) override;
  void markPage(uint64_t virtualAddress, bool value) override;
  bool mapPage(uint64_t physicalAddress, uint64_t virtualAddress) override;
};