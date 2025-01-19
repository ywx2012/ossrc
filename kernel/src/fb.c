#include <x86/io.h>
#include <pci.h>
#include <kernel/bsp.h>
#include <kernel/paging.h>
#include <kernel/shm.h>

#define VBE_INDEX_PORT 0x01ce
#define VBE_DATA_PORT 0x01cf
#define VBE_INDEX_ID 0x0
#define VBE_INDEX_XRES 0x1
#define VBE_INDEX_YRES 0x2
#define VBE_INDEX_BPP 0x3
#define VBE_INDEX_ENABLE 0x4

#define FB_XRES 640
#define FB_YRES 480
#define FB_BPP  32

static struct shm fb1;

void
fb_init(void) {
  outw(VBE_INDEX_ID, VBE_INDEX_PORT);
  if (inw(VBE_DATA_PORT) < 0xb0c0)
    return;
  outw(VBE_INDEX_XRES, VBE_INDEX_PORT);
  outw(FB_XRES, VBE_DATA_PORT);
  outw(VBE_INDEX_YRES, VBE_INDEX_PORT);
  outw(FB_YRES, VBE_DATA_PORT);
  outw(VBE_INDEX_BPP, VBE_INDEX_PORT);
  outw(FB_BPP, VBE_DATA_PORT);
  outw(VBE_INDEX_ENABLE, VBE_INDEX_PORT);
  outw(1, VBE_DATA_PORT);

  uint16_t addr = pci_find_device(0x1234, 0x1111, 0x1af4, 0x1100);
  uint32_t fb_base = pci_readl(addr, PCI_BASE_ADDRESS_0) & 0xFFFFFFF0;

  fb1.info.size = FB_XRES * FB_YRES * sizeof(uint32_t);
  fb1.info.xres = FB_XRES;
  fb1.info.yres = FB_YRES;
  fb1.pte = fb_base|PTE_W;
  shm_create(&fb1, "fb-1");
}
