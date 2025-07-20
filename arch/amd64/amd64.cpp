#include "amd64.hpp"
#include "arch/amd64/interupt/IDT.hpp"
#include <cstdint>

Amd64::~Amd64() {}

void Amd64::out(uint16_t port, uint8_t value) {
  __asm__ volatile("outb %b0, %w1" : : "a"(value), "Nd"(port) : "memory");
}

void Amd64::out(uint16_t port, uint16_t value) {
  __asm__ volatile("outw %w0, %w1" : : "a"(value), "Nd"(port) : "memory");
}

void Amd64::out(uint16_t port, uint32_t value) {
  __asm__ volatile("outl %d0, %w1" : : "a"(value), "Nd"(port) : "memory");
}

uint8_t Amd64::inb(uint16_t port) {
  uint8_t res = 0;
  __asm__ volatile("inb %w1, %b0" : "=a"(res) : "Nd"(port) : "memory");
  return res;
}

uint16_t Amd64::inw(uint16_t port) {
  uint16_t res = 0;
  __asm__ volatile("inw %w1, %w0" : "=a"(res) : "Nd"(port) : "memory");
  return res;
}

uint32_t Amd64::ind(uint16_t port) {
  uint32_t res = 0;
  __asm__ volatile("inl %w1, %d0" : "=a"(res) : "Nd"(port) : "memory");
  return res;
}

void Amd64::init() { AMD64::initKernelIDTTab(); }

void Amd64::setInterrupt(int n, int type, int dpl, void *offset) {
  if (type == Gradidierite::INTERRUPT_EXECEPTION) {
    AMD64::kernelIDTTab[n] = AMD64::createEntry(
        offset, 0x8, 0, PRESENT | TRAP_GATE | ((uint8_t)dpl << 4));
  }
}