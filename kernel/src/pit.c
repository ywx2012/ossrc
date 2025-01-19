#include <x86/io.h>
#include <x86/pit.h>

#define IOPORT_CONTROL 0x43
#define LOBYTE 0x10
#define HIBYTE 0x20
#define PIT_BINARY_MODE 0x0

static unsigned char const channels[] = {0x0, 0x40, 0x80};
static unsigned short const ioport[] = {0x40, 0x41, 0x42};

void
pit_set_mode(unsigned char channel, unsigned char mode) {
  outb(channels[channel]|LOBYTE|HIBYTE|mode|PIT_BINARY_MODE, IOPORT_CONTROL);
}

void
pit_set_count(unsigned char channel, unsigned short count) {
  unsigned char lo = count & 0xFFU;
  unsigned char hi = (count >> 8) & 0xFFU;
  outb(lo, ioport[channel]);
  outb(hi, ioport[channel]);
}
