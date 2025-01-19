#pragma once
#include <x86/segment.h>

struct setup_header {
  uint8_t    setup_sects;
  uint16_t   root_flags;
  uint32_t   syssize;
  uint16_t   ram_size;
  uint16_t   vid_mode;
  uint16_t   root_dev;
  uint16_t   boot_flag;
  uint16_t   jump;
  uint32_t   header;
  uint16_t   version;
  uint32_t   realmode_swtch;
  uint16_t   start_sys_seg;
  uint16_t   kernel_version;
  uint8_t    type_of_loader;
  uint8_t    loadflags;
  uint16_t   setup_move_size;
  uint32_t   code32_start;
  uint32_t   ramdisk_image;
  uint32_t   ramdisk_size;
  uint32_t   bootsect_kludge;
  uint16_t   heap_end_ptr;
  uint8_t    ext_loader_ver;
  uint8_t    ext_loader_type;
  uint32_t   cmd_line_ptr;
  uint32_t   initrd_addr_max;
  uint32_t   kernel_alignment;
  uint8_t    relocatable_kernel;
  uint8_t    min_alignment;
  uint16_t   xloadflags;
  uint32_t   cmdline_size;
  uint32_t   hardware_subarch;
  uint64_t   hardware_subarch_data;
  uint32_t   payload_offset;
  uint32_t   payload_length;
  uint64_t   setup_data;
  uint64_t   pref_address;
  uint32_t   init_size;
  uint32_t   handover_offset;
  uint32_t   kernel_info_offset;
} __attribute__((packed));

#define E820MAX 128

struct e820entry {
  uint64_t addr;
  uint64_t size;
  uint32_t type;
} __attribute__((packed));

struct e820map {
  uint32_t nr_map;
  struct e820entry map[E820MAX];
};

#define E820_RAM 1

extern struct setup_header setup_header;
extern struct e820map e820map;
extern struct dtr gdtr;
extern uint8_t font_data[128*16];

void setup_start(void);
