#include "boot/adachiite.h"
#include "kernel/kprintf/kprintf.hpp"
#include <cstdint>
#include <stdint.h>

namespace MemoryManagement {

typedef struct {
  uint64_t offset;
  uint8_t freePages;
  uint8_t nPages;
  uint64_t bitmap;
} BlockHeader;

uint64_t freePages = 0;
uint64_t numberOfBlocks = 0;
uint64_t pageUsedByPA = 0;
uint64_t blockHeadersPage = 0; // where block headers at

void pageAllocationInit() {
  // check how many pages are free
  MemMapDesc *memmap = (MemMapDesc *)adachiiteBootInfo->memoryMap;
  uint64_t curSize = 0;
  while (curSize < adachiiteBootInfo->memoryMapSize) {
    if (memmap->Type == USABLE)
      freePages += memmap->NumberOfPages;

    memmap = (MemMapDesc *)((char *)memmap + adachiiteBootInfo->memoryDescSize);
    curSize += adachiiteBootInfo->memoryDescSize;
  }

  // calculate how many HeaderBlock need
  numberOfBlocks = (freePages / 64) + (freePages % 64 != 0);
  pageUsedByPA = (numberOfBlocks * sizeof(BlockHeader) / 4096);
  if (numberOfBlocks * sizeof(BlockHeader) / 4096 != 0)
    pageUsedByPA++;
  kprintf("Number of free pages: %u, num of blocks: %u ,page need: %u\n",
          freePages, numberOfBlocks, pageUsedByPA);

  // loop through memmap again and find continuous area for block headers
  curSize = 0;
  while (curSize < adachiiteBootInfo->memoryMapSize) {
    if (memmap->Type == USABLE && memmap->NumberOfPages >= pageUsedByPA) {
      blockHeadersPage = memmap->PhysicalStart;
    }

    memmap = (MemMapDesc *)((char *)memmap + adachiiteBootInfo->memoryDescSize);
    curSize += adachiiteBootInfo->memoryDescSize;
  }
}

} // namespace MemoryManagement