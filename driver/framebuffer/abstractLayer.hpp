#pragma once

#include "driver/driver.hpp"
#include <cstdint>

class FramebufferDriverAbstractLayer : public GrandidieriteDriverAbstractLayer {
public:
  uint32_t getType() override;

  virtual void putPixel(uint32_t color, uint32_t x, uint32_t y) = 0;
  // when use double buffering we need this
  virtual void flush() = 0;
  virtual uint32_t getHeight() = 0;
  virtual uint32_t getWidth() = 0;
  ~FramebufferDriverAbstractLayer();
  FramebufferDriverAbstractLayer();
};