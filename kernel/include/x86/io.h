#pragma once

static inline
unsigned char
inb (unsigned short port) {
  unsigned char value;
  __asm__ ("inb %w1,%b0" : "=a"(value) : "d"(port));
  return value;
}

static inline
void
outb(unsigned char value, unsigned short port) {
  __asm__ ("outb %b0,%w1" : : "a"(value), "d"(port));
}

static inline
unsigned short
inw (unsigned short port) {
  unsigned short value;
  __asm__ ("inw %w1,%w0" : "=a"(value) : "d"(port));
  return value;
}

static inline
void
outw(unsigned short value, unsigned short port) {
  __asm__ ("outw %w0,%w1" : : "a"(value), "d"(port));
}

static inline
unsigned
inl(unsigned short port) {
  unsigned value;
  __asm__ ("inl %w1,%0" : "=a" (value) : "d" (port));
  return value;
}

static inline
void
outl(unsigned value, unsigned short int port) {
  __asm__ ("outl %0,%w1" : : "a" (value), "d" (port));
}
