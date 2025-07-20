#include "driver.hpp"
#include "driver/debug/qemu/serialDebug.hpp"

QemuSerialDebug qemuSerialDebug;
DebugAbstractLayer *debugDevice = &qemuSerialDebug;

// init static driver class
void initDriver() {}