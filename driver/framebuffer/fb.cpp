#include "abstractLayer.hpp"
#include "driver/driver.hpp"
#include <cstdint>

uint32_t FramebufferDriverAbstractLayer::getType() {
  return SIMPLE_FRAMEBUFFER_DRIVER;
}

FramebufferDriverAbstractLayer::FramebufferDriverAbstractLayer() {}

FramebufferDriverAbstractLayer::~FramebufferDriverAbstractLayer() {}
