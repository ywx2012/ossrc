#include <sys/io.h>
#include <pci.h>
#include <fb.h>
#include <bsp.h>
#include <paging.h>
#include <task.h>

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

static uint32_t fb_base;

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
  fb_base = pci_readl(addr, PCI_BASE_ADDRESS_0) & 0xFFFFFFF0;
}

unsigned long
do_get_fb_info(struct fb_info *fb_info) {
  fb_info->xres = FB_XRES;
  fb_info->yres = FB_YRES;
  return 0;
}

unsigned long
do_fbmap() {
  unsigned long va = 0xe000000;
  unsigned long pa = fb_base;
  unsigned long size = FB_XRES * FB_YRES * 4;
  for(unsigned long offset = 0; offset<size; offset+=PAGE_SIZE)
    paging_map_addr(current->pml4, va+offset, pa+offset, PTE_W|PTE_U);
  return va;
}
