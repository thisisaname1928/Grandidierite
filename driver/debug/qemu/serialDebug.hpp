#include "../abstractLayer.hpp"
#pragma once

class QemuSerialDebug : public DebugAbstractLayer {
  const char *getName() override;
  void putc(char) override;
};
