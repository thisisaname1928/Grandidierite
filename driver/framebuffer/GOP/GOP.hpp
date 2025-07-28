#pragma once
#include "driver/framebuffer/abstractLayer.hpp"
#include <cstdint>

class GOPFramebufferDriver : public FramebufferDriverAbstractLayer {
public:
  uint32_t getType() override;
  const char *getName() override;
  bool init() override;
  void putPixel(uint32_t color, uint32_t x, uint32_t y) override;
  void flush() override;
  uint32_t getHeight() override;
  uint32_t getWidth() override;
  ~GOPFramebufferDriver();
  GOPFramebufferDriver();
};