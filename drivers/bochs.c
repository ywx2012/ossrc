#include <sys/io.h>
#include "include/pci.h"
#include "include/bochs.h"

#define VBE_INDEX_PORT 0x01ce
#define VBE_DATA_PORT 0x01cf
#define VBE_INDEX_ID 0x0
#define VBE_INDEX_XRES 0x1
#define VBE_INDEX_YRES 0x2
#define VBE_INDEX_BPP 0x3
#define VBE_INDEX_ENABLE 0x4

void bochs_init(struct vesa_mode_info *mode_info) {
  outw(VBE_INDEX_ID, VBE_INDEX_PORT);
  if (inw(VBE_DATA_PORT) < 0xb0c0)
    return;

  outw(VBE_INDEX_XRES, VBE_INDEX_PORT);
  outw(640, VBE_DATA_PORT);
  outw(VBE_INDEX_YRES, VBE_INDEX_PORT);
  outw(480, VBE_DATA_PORT);
  outw(VBE_INDEX_BPP, VBE_INDEX_PORT);
  outw(32, VBE_DATA_PORT);
  outw(VBE_INDEX_ENABLE, VBE_INDEX_PORT);
  outw(1, VBE_DATA_PORT);

  uint16_t addr = pci_find_device(0x1234, 0x1111, 0x1af4, 0x1100);
  uint32_t base = pci_readl(addr, PCI_BASE_ADDRESS_0) & 0xFFFFFFF0;
  mode_info->fbbase = base;
  mode_info->hres = 640;
  mode_info->vres = 480;
  mode_info->bpp = 32;
}
