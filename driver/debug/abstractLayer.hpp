#pragma once

class DebugAbstractLayer {
public:
  virtual const char *getName() = 0;
  virtual void putc(char c) = 0;
};