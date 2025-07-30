#include "arch/arch.hpp"
#include "boot/adachiite.h"
#include "kernel/kprintf/kprintf.hpp"
#include <cstdint>
#include <stdint.h>

namespace MemoryManagement {

// set first bit of offset is 1
#define HEADER_USED 1

typedef struct {
  uint64_t offset;
  uint64_t size; // in 4KB
} BlockHeader;

uint64_t freePages = 0;
uint64_t numberOfBlocks = 0;
uint64_t pageUsedByPA = 0;
uint64_t blockHeadersPage = 0; // where block headers at

uint64_t truncAddress(uint64_t value) { return value & 0xffffffffff000; }
bool isHeaderFree(BlockHeader hdr) { return (hdr.offset & 1) == 1; }

uint64_t findFreeBlockHeader() {
  BlockHeader *hdrs = (BlockHeader *)blockHeadersPage;
  BlockHeader *subPtr = (BlockHeader *)(KERNEL_VIRTUAL_ADDRESS + 0x1f400000);
  for (uint64_t i = 0; i < numberOfBlocks; i++) {
    if (i % 256 == 0)
      arch->mapPage((uint64_t)&hdrs[i], KERNEL_VIRTUAL_ADDRESS + 0x1f400000);

    if (isHeaderFree(subPtr[i % 256])) {
      return i;
    }
  }

  kprintf("SO WRONG!\n");
  return 0;
}

// map block header to KERNEL_VIRTUAL_ADDRESS + 0x1f400000
uint16_t mapBlockHeader(uint64_t idx) {
  BlockHeader *hdrs = (BlockHeader *)blockHeadersPage;
  arch->mapPage((uint64_t)&hdrs[idx], KERNEL_VIRTUAL_ADDRESS + 0x1f400000);

  // return the offset
  return idx % 256;
}

bool isPowerOf2(uint64_t value) {
  if (value == 1 || value == 0)
    return false;

  // like 0b1000 is power of 2, 0b1000 - 1 = 0b0111
  return (value & (value - 1)) == 0;
}

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

      // dug a hole in memMap
      memmap->PhysicalStart += pageUsedByPA * 0x1000;
      memmap->NumberOfPages -= pageUsedByPA;
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
  // map hdrs[i] address to 500MB above kernel virtAddr then fill their offset
  // with zero because we can't make sure that their memory region mapped 1:1
  // we will use this alot
  for (uint64_t i = 0; i < numberOfBlocks; i++) {
    if (i % 256 == 0)
      arch->mapPage((uint64_t)&hdrs[i], KERNEL_VIRTUAL_ADDRESS + 0x1f400000);

    subPtr[i % 256].offset = i;
  }

  // loop through memmap again and find some info about free memory for the
  // allocation
  memmap = (MemMapDesc *)adachiiteBootInfo->memoryMap;
  curSize = 0;
  while (curSize < adachiiteBootInfo->memoryMapSize) {
    uint64_t sz = 0;
    if (memmap->Type == USABLE) {
      if (memmap->PhysicalStart != blockHeadersPage) {
        sz = memmap->NumberOfPages;
        while (sz > 0 && !isPowerOf2(sz))
          sz--;
      }

      if (sz > 0) {
        uint16_t idx = mapBlockHeader(findFreeBlockHeader());
        subPtr[idx].offset = memmap->PhysicalStart;
        subPtr[idx].size = sz;
        kprintf("offset: %x, size: %u\n", subPtr[idx].offset, subPtr[idx].size);
      }
    }

    memmap = (MemMapDesc *)((char *)memmap + adachiiteBootInfo->memoryDescSize);
    curSize += adachiiteBootInfo->memoryDescSize;
  }

  return true;
}

} // namespace MemoryManagement