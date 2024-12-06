
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <stddef.h>
#include <print.h>
#include <shm.h>
#include <timer.h>

#define AQUA    0x00FFFF
#define FUCHSIA 0xFF00FF
#define YELLOW  0xFFFF00

static struct shm_info fb_info;
static uint32_t *fbbase = (uint32_t *)0xe000000;
static struct shm_info font_info;
static uint8_t const *font_data = (uint8_t *)0x2000000;

static
uint8_t const *
get_glyph(char index) {
  uint16_t width = font_info.xres;
  return font_data + ((width + 7) / 8) * font_info.yres * index;
}

static
void
draw_char(unsigned x, unsigned y, char index, uint32_t color) {
  uint8_t const *glyph = get_glyph(index);
  uint16_t width = font_info.xres;
  uint16_t height = font_info.yres;
  for (unsigned i=y; i<y+height; ++i) {
    for (unsigned j=0; j<width; j=(uint16_t)(j+8)) {
      uint8_t bits = *glyph;
      for (unsigned k=0; (k < 8) && ((j+k) < width); ++k)
        if ((bits<<k)&0x80)
          fbbase[fb_info.xres * i + x + j + k] = color;
      glyph += 1;
    }
  }
}

int
main(void) {
  char *m = (char *)0x4000000;
  shm_map("shm-1", m);
  shm_get_info("fb-1", &fb_info);
  shm_map("fb-1", fbbase);
  shm_get_info("font-1", &font_info);
  shm_map("font-1", font_data);

  uint32_t color[] = {AQUA, FUCHSIA, YELLOW};
  char const *s = "Hello, world!";

  for(;;) {
    for (size_t i=0; i<sizeof(color)/sizeof(color[0]); ++i) {
      uint32_t c = color[i];
      for (unsigned j=0; s[j]; ++j)
        draw_char((j+1)*font_info.xres, font_info.xres/2, s[j], c);
      print(*m);
      timer_sleep(1000);
    }
  }
}
