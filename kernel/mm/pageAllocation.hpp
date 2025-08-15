#pragma once
#include <cstdint>

namespace MemoryManagement {
bool pageAllocationInit();
void *allocPages(uint64_t *nPages);
} // namespace MemoryManagement