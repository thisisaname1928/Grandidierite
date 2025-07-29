
#include "arch/amd64/amd64.hpp"
#include "kernel/kprintf/kprintf.hpp"
#include <cstdint>
#include <stdint.h>

extern "C" uint64_t getCr3();
extern "C" void reloadTLB(uint64_t);
#define HUGE_PAGE (1 << 7)

uint64_t truncAddress(uint64_t value) { return value & 0xffffffffff000; }

// function for check if address is available
bool Amd64::checkMappingAddress(uint64_t virtualAddress) {
  uint64_t *PML4 = (uint64_t *)getCr3();
  uint16_t PML4Index = (virtualAddress >> 39) & 0x1ff; // get 8 bit index
  // check present bit
  if (!(PML4[PML4Index] & 1))
    return false;
  // get 52 bit address and remove first 8 bit
  uint64_t *PDP = (uint64_t *)(truncAddress(PML4[PML4Index]));
  uint16_t PDPIndex = (virtualAddress >> 30) & 0x1ff;

  if (!(PDP[PDPIndex] & 1))
    return false;

  // if it is a huge page then
  if ((PDP[PDPIndex] & HUGE_PAGE) == HUGE_PAGE)
    return true;

  // check PD like how I checked PDP
  uint64_t *PD = (uint64_t *)(truncAddress(PDP[PDPIndex]));
  uint16_t PDIndex = (virtualAddress >> 21) & 0x1ff;

  if (!(PD[PDIndex] & 1))
    return false;

  // if it is a huge page then
  if ((PD[PDIndex] & HUGE_PAGE) == HUGE_PAGE)
    return true;

  // check PT
  uint64_t *PT = (uint64_t *)(truncAddress(PD[PDIndex]));
  uint16_t PTIndex = (virtualAddress >> 12) & 0x1ff;

  return PT[PTIndex] & 1;
}

uint64_t *getPage(uint64_t virtualAddress) {
  uint64_t *PML4 = (uint64_t *)getCr3();
  uint16_t PML4Index = (virtualAddress >> 39) & 0x1ff;

  if (!(PML4[PML4Index] & 1))
    return 0;

  uint64_t *PDP = (uint64_t *)(truncAddress(PML4[PML4Index]));
  uint16_t PDPIndex = (virtualAddress >> 30) & 0x1ff;

  if (!(PDP[PDPIndex] & 1))
    return 0;

  if ((PDP[PDPIndex] & HUGE_PAGE) == HUGE_PAGE)
    return &PDP[PDPIndex];

  uint64_t *PD = (uint64_t *)(truncAddress(PDP[PDPIndex]));
  uint16_t PDIndex = (virtualAddress >> 21) & 0x1ff;

  if (!(PD[PDIndex] & 1))
    return 0;

  if ((PD[PDIndex] & HUGE_PAGE) == HUGE_PAGE)
    return &PD[PDIndex];

  uint64_t *PT = (uint64_t *)(truncAddress(PD[PDIndex]));
  uint16_t PTIndex = (virtualAddress >> 12) & 0x1ff;

  return &PT[PTIndex];
}

// check if page available
// only work if every page is 4KB mapped
bool Amd64::checkPageStatus(uint64_t virtualAddress) {
  if (!checkMappingAddress(virtualAddress))
    return true;

  uint64_t *page = getPage(virtualAddress);
  if (page == 0)
    return 0;

  // I set bit 8 of PT as a sign if page used
  return (*page >> 8) & 1;
}

void Amd64::markPage(uint64_t virtualAddress, bool value) {
  uint64_t *page = getPage(virtualAddress);
  if (page != 0) {
    if (value)
      *page |= (1 << 8);
    if (!value) {
      *page &= ~(1 << 8);
    }
  }
}

bool Amd64::mapPage(uint64_t physicalAddress, uint64_t virtualAddress) {
  uint64_t *page = getPage(virtualAddress);
  if (page == 0)
    return false;

  *page = (*page & ~truncAddress(*page)) | truncAddress(physicalAddress);
  kprintf("%x\n", *page);
  reloadTLB(virtualAddress);
  return true;
}