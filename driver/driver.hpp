#include "debug/abstractLayer.hpp"
#include <cstdint>
#pragma once

extern DebugAbstractLayer *debugDevice;
void initDriver();

typedef enum {
  DEBUG_CONSOLE_DRIVER = 1,
  SIMPLE_FRAMEBUFFER_DRIVER = 2
} DRIVER_TYPE;

class GrandidieriteDriverAbstractLayer {
public:
  virtual uint32_t getType() = 0;
  virtual const char *getName() = 0;
  virtual bool init() = 0;
  ~GrandidieriteDriverAbstractLayer();
  GrandidieriteDriverAbstractLayer();
};