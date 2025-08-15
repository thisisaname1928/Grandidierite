#include "arch/arch.hpp"
#include "boot/adachiite.h"
#include "kernel/kprintf/kprintf.hpp"
#include <cstdint>
#include <stdint.h>

namespace MemoryManagement {

// use to mark some block isn't free
#define NOT_FREE 1

typedef struct {
  uint64_t offset;
  uint64_t size; // in 4KB
} BlockHeader;

// number of pages
uint64_t pages = 0;
uint64_t numberOfBlocks = 0;
uint64_t pageUsedByPA = 0;
uint64_t blockHeadersPage = 0; // where block headers at

uint64_t truncAddress(uint64_t value) { return value & 0xffffffffff000; }
bool isHeaderFree(BlockHeader hdr) { return (hdr.offset & 1) != 1; }

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
  // check how many pages are in the memory
  MemMapDesc *memmap = (MemMapDesc *)adachiiteBootInfo->memoryMap;
  uint64_t curSize = 0;
  while (curSize < adachiiteBootInfo->memoryMapSize) {
    pages += memmap->NumberOfPages;
    if (memmap->Type == USABLE) {
      kprintf("%x: %u\n", memmap->PhysicalStart, memmap->NumberOfPages);
    }

    memmap = (MemMapDesc *)((char *)memmap + adachiiteBootInfo->memoryDescSize);
    curSize += adachiiteBootInfo->memoryDescSize;
  }

  // calculate how many HeaderBlock need, at worst case, we will need number of
  // headers = pages
  numberOfBlocks = pages;
  pageUsedByPA = (numberOfBlocks * sizeof(BlockHeader) / 4096);
  if ((numberOfBlocks * sizeof(BlockHeader)) % 4096 != 0)
    pageUsedByPA++;
  kprintf("Number of pages: %u, num of blocks: %u, page need: %u\n", pages,
          numberOfBlocks, pageUsedByPA);

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

  kprintf("first block header start at: %x, size of headers in MB: %u\n",
          blockHeadersPage, pageUsedByPA * 4 / 1024);

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

  // loop through memmap again and find some info about memory for the
  // allocation
  memmap = (MemMapDesc *)adachiiteBootInfo->memoryMap;
  curSize = 0;
  uint64_t lastBlockIdx = 0;
  uint64_t lastBlockSz = 0;
  while (curSize < adachiiteBootInfo->memoryMapSize) {
    uint64_t sz = memmap->NumberOfPages;
    uint64_t startAddress = memmap->PhysicalStart;

    if (lastBlockIdx == startAddress) // if first block have some free page
      lastBlockSz = sz;

    if (memmap->Type == USABLE) {
      if (memmap->PhysicalStart ==
          blockHeadersPage) { // check if it used by allocator
        uint16_t idx = mapBlockHeader(truncAddress(startAddress) / 4096);
        subPtr[idx].offset = startAddress | NOT_FREE;
        subPtr[idx].size = pageUsedByPA;

        startAddress += pageUsedByPA * 4096;
        sz = sz - pageUsedByPA;
      }

      // a loop to make sure all block are power of 2
      while (sz > 1) {
        uint64_t currentPageSize = sz; // make a backup of sz
        while (sz > 1 && !isPowerOf2(sz))
          sz--;

        if (sz > 1) {
          uint16_t idx = mapBlockHeader(truncAddress(startAddress) / 4096);
          subPtr[idx].offset = startAddress;
          subPtr[idx].size = sz;

          kprintf("[BUDDY]: offset: %x, size: %u\n",
                  subPtr[idx].offset & ~0xfff, sz);

          // increase
          startAddress += sz * 4096;
          sz = currentPageSize - sz;
        }
      }
    } else { // we dont need to make sure it must be a power of 2
      uint16_t idx = mapBlockHeader(truncAddress(startAddress) / 4096);
      subPtr[idx].offset = startAddress | NOT_FREE;
      subPtr[idx].size = sz;
    }

    if (lastBlockIdx * 4096 + lastBlockSz * 4096 !=
        startAddress) { // make sure some memory gap is listed
      uint16_t idx = mapBlockHeader(
          lastBlockIdx + lastBlockSz); // get beginning page of the gap
      subPtr[idx].offset =
          (lastBlockIdx * 4096 + lastBlockSz * 4096) | NOT_FREE;
      subPtr[idx].size =
          (startAddress - (lastBlockIdx * 4096 + lastBlockSz * 4096)) / 4096;

      kprintf("mem gap: offset=%x, sz=%u\n", subPtr[idx].offset & ~0xfff,
              subPtr[idx].size);
    }

    lastBlockIdx = truncAddress(startAddress) / 4096;
    lastBlockSz = sz;

    memmap = (MemMapDesc *)((char *)memmap + adachiiteBootInfo->memoryDescSize);
    curSize += adachiiteBootInfo->memoryDescSize;
  }

  kprintf("pageAllocation said ok!\n");

  return true;
}

void *allocPages(uint64_t *nPages) {
  uint64_t i = 0;
  BlockHeader *subPtr = (BlockHeader *)(KERNEL_VIRTUAL_ADDRESS + 0x1f400000);
  while (i < pages) {
    BlockHeader *hdrs = (BlockHeader *)blockHeadersPage;
    if (i % 256 == 0)
      arch->mapPage((uint64_t)&hdrs[i], KERNEL_VIRTUAL_ADDRESS + 0x1f400000);

    if (isHeaderFree(subPtr[i % 256]) && subPtr[i % 256].size >= *nPages) {
      break;
    } else {
      i += subPtr[i % 256].size;
      kprintf("L: %x\n", i * 4096);
    }
  }

  kprintf("use page %x, size=%u\n", i * 4096, subPtr[i % 256].size);

  *nPages = 0;
  return 0;
}

} // namespace MemoryManagement