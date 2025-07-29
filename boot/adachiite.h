#include <stdint.h>
#ifndef PROTOCOL_H
#define PROTOCOL_H

#define KERNEL_VIRTUAL_ADDRESS 0xffffffff80000000
typedef enum { BIOS = 1, UEFI = 2 } FIRMWARE_PLATFORM;
typedef enum { FIRST_ADACHIITE = 0x11 } REVISION;

typedef enum { ARGB = 0, BGRA = 1 } COLOR_FORMAT;

typedef struct {
  uint64_t horizontalResolution;
  uint64_t verticalResolution;
  uint64_t framebuffer;
  uint64_t colorFormat;
} framebuffer;

typedef enum {
  RESERVED = 0,
  BOOTLOADER = 1,
  UEFIDATA,
  USABLE,
  ACPI,
  MEMORY_MAPPEDIO,
  PERSISTENT
} MEMORYTYPE;

// use the same struct with uefi:)
typedef struct {
  uint32_t Type;
  uint32_t Pad;
  uint64_t PhysicalStart;
  uint64_t VirtualStart;
  uint64_t NumberOfPages;
  uint64_t Attribute;
} MemMapDesc;

typedef struct {
  uint16_t revision;
  uint16_t firmwarePlatform;
  framebuffer fb;
  uint64_t memoryMap;
  uint64_t memoryMapSize;
  uint64_t memoryDescSize;
  uint64_t kernelPhysicalAddress;
  uint64_t kernelSize; // in 4KB
  uint64_t initPhysicalAddress;
  uint64_t initSize; // in bytes
} AdachiiteBootInfo;

extern AdachiiteBootInfo *adachiiteBootInfo;
#endif