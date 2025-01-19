#pragma clang section text=".boot.text" rodata=".boot.rodata" data=".boot.data" bss=".boot.bss"

#include <bios.h>

void
bios_load_map(struct e820map *map) {
  // https://uefi.org/htmlspecs/ACPI_Spec_6_4_html/15_System_Address_Map_Interfaces/int-15h-e820h---query-system-address-map.html
  uint32_t nr_map = 0;
  uint32_t sig = 0x0534D4150; // SMAP
  uint32_t cont = 0;
  do {
    uint32_t size = sizeof(struct e820entry);
    uint32_t ptr = (uint32_t)&map->map[nr_map];
    int cf = 0;
    __asm__("int $0x15"
            : "=a"(sig), "=b"(cont), "=c"(size), "=D"(ptr), "=@ccc"(cf)
            : "a"(0xe820), "b"(cont), "c"(size), "d"(sig), "D"(ptr)
            : "memory");
    ++nr_map;
    if (cf || (sig != 0x0534D4150)) {
      nr_map = 0;
      break;
    }
  } while (cont != 0);
  map->nr_map = nr_map;
}

void
bios_load_font(uint8_t *font) {
  __asm__("pushl %%ebp\n"
          "pushl %%es\n"
          "pushl %%ds\n" // [ds0 es0 ebp0]
          "int $0x10\n"
          "pushl %%es\n" // [es1 ds0 es0 ebp0]
          "popl %%ds\n" // ds = es1
          "popl %%es\n" // es = ds0
          "mov %%bp, %%si\n"
          "mov $%c0, %%cx\n"
          "rep movsb\n" // mov byte at ds:si to es:di
          "pushl %%es\n" // [ds0 es0 ebp0]
          "popl %%ds\n"
          "popl %%es\n"
          "popl %%ebp\n"
          :
          : "i"(16*128), "a"(0x1130), "b"(0x0600), "D"(font)
          : "cx", "dx", "si", "memory");
}
