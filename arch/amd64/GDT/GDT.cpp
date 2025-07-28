#include "GDT.hpp"
#include <cstdint>

namespace AMD64 {
// it always work better when being loaded by using asm:)
const uint64_t CS = 0x8;
extern "C" void loadGDT();
void initGDT() { loadGDT(); }
} // namespace AMD64