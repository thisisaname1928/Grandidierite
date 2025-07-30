#include "arch/arch.hpp"
#include "boot/adachiite.h"
#include "kernel/kprintf/kprintf.hpp"
#include <cstdint>
#include <stdint.h>

namespace MemoryManagement {

typedef struct {
  uint64_t offset;
  uint64_t size;
} BlockHeader;

uint64_t freePages = 0;
uint64_t numberOfBlocks = 0;
uint64_t pageUsedByPA = 0;
uint64_t blockHeadersPage = 0; // where block headers at

uint64_t truncAddress(uint64_t value) { return value & 0xffffffffff000; }

bool pageAllocationInit() {
  // check how many pages are free
  MemMapDesc *memmap = (MemMapDesc *)adachiiteBootInfo->memoryMap;
  uint64_t curSize = 0;
  while (curSize < adachiiteBootInfo->memoryMapSize) {
    if (memmap->Type == USABLE) {
      freePages += memmap->NumberOfPages;
      kprintf("%x: %u\n", memmap->PhysicalStart, memmap->NumberOfPages);
    }

    memmap = (MemMapDesc *)((char *)memmap + adachiiteBootInfo->memoryDescSize);
    curSize += adachiiteBootInfo->memoryDescSize;
  }

  // calculate how many HeaderBlock need, at worst case, we will need number of
  // freePages / 2
  numberOfBlocks = freePages / 2;
  pageUsedByPA = (numberOfBlocks * sizeof(BlockHeader) / 4096);
  if ((numberOfBlocks * sizeof(BlockHeader)) % 4096 != 0)
    pageUsedByPA++;
  kprintf("Number of free pages: %u, num of blocks: %u, page need: %u\n",
          freePages, numberOfBlocks, pageUsedByPA);

  // loop through memmap again and find continuous area for block headers
  memmap = (MemMapDesc *)adachiiteBootInfo->memoryMap;
  curSize = 0;
  bool found = false;
  while (curSize < adachiiteBootInfo->memoryMapSize) {
    if (memmap->Type == USABLE && memmap->NumberOfPages >= pageUsedByPA) {
      blockHeadersPage = memmap->PhysicalStart;
      found = true;
      break;
    }

    memmap = (MemMapDesc *)((char *)memmap + adachiiteBootInfo->memoryDescSize);
    curSize += adachiiteBootInfo->memoryDescSize;
  }

  if (!found) {
    kprintf("can't found continous pages for allocator\n");
    return false;
  }

  kprintf("first block header start at: %x, size in bytes: %u\n",
          blockHeadersPage, pageUsedByPA * 4096);

  // init headers
  BlockHeader *hdrs = (BlockHeader *)blockHeadersPage;
  BlockHeader *subPtr = (BlockHeader *)(KERNEL_VIRTUAL_ADDRESS + 0x1f400000);
  // map hdrs[i] address to 500MB above kernel virtAddr
  for (uint64_t i = 0; i < numberOfBlocks; i++) {
    if (i % 256 == 0)
      arch->mapPage((uint64_t)&hdrs[i], KERNEL_VIRTUAL_ADDRESS + 0x1f400000);

    subPtr[i % 256].offset = i;
  }

  return true;
}

} // namespace MemoryManagement