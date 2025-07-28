#include "GOP.hpp"
#include "boot/adachiite.h"
#include "driver/driver.hpp"
#include <cstdint>

namespace GOP {
uint32_t *framebuffer;
uint32_t width = 0, height = 0;
} // namespace GOP

// get info from adachiite
bool GOPFramebufferDriver::init() {
  if (adachiiteBootInfo->firmwarePlatform != UEFI)
    return false;

  GOP::framebuffer = (uint32_t *)adachiiteBootInfo->fb.framebuffer;
  GOP::height = adachiiteBootInfo->fb.verticalResolution;
  GOP::width = adachiiteBootInfo->fb.horizontalResolution;

  return true;
}

uint32_t GOPFramebufferDriver::getHeight() { return GOP::width; }

uint32_t GOPFramebufferDriver::getWidth() { return GOP::height; }

// currently I haven't implement a memory allocation so this is empty
void GOPFramebufferDriver::flush() {}

void GOPFramebufferDriver::putPixel(uint32_t color, uint32_t x, uint32_t y) {
  GOP::framebuffer[GOP::width * y + x] = color;
}

uint32_t GOPFramebufferDriver::getType() { return SIMPLE_FRAMEBUFFER_DRIVER; }
const char *GOPFramebufferDriver::getName() {
  return "A simple gop framebuffer driver";
}

GOPFramebufferDriver::~GOPFramebufferDriver() {}
GOPFramebufferDriver::GOPFramebufferDriver() {}