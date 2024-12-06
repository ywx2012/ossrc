
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <stddef.h>
#include <print.h>
#include <shm.h>
#include <fb.h>
#include <timer.h>

#define AQUA    0x00FFFF
#define FUCHSIA 0xFF00FF
#define YELLOW  0xFFFF00

static struct fb_info fb_info;
static uint32_t *fbbase = (uint32_t *)0xe000000;

static const uint8_t fonts_zh[2][32] = {
  {
    // 中
    0x01, 0x80, /* 0000000110000000 */
    0x01, 0x80, /* 0000000110000000 */
    0x01, 0x80, /* 0000000110000000 */
    0x01, 0x80, /* 0000000110000000 */
    0xff, 0xff, /* 1111111111111111 */
    0xc1, 0x83, /* 1100000110000011 */
    0xc1, 0x83, /* 1100000110000011 */
    0xc1, 0x83, /* 1100000110000011 */
    0xc1, 0x83, /* 1100000110000011 */
    0xc1, 0x83, /* 1100000110000011 */
    0xc1, 0x83, /* 1100000110000011 */
    0xff, 0xff, /* 1111111111111111 */
    0x01, 0x80, /* 0000000110000000 */
    0x01, 0x80, /* 0000000110000000 */
    0x01, 0x80, /* 0000000110000000 */
    0x01, 0x80, /* 0000000110000000 */
  },
  // 国
  {
    0xff, 0xff, /* 1111111111111111 */
    0xc0, 0x03, /* 1100000000000011 */
    0xc0, 0x03, /* 1100000000000011 */
    0xcf, 0xf3, /* 1100111111110011 */
    0xc1, 0x83, /* 1100000110000011 */
    0xc1, 0x83, /* 1100000110000011 */
    0xc1, 0x83, /* 1100000110000011 */
    0xc7, 0xe3, /* 1100011111100011 */
    0xc1, 0x83, /* 1100000110000011 */
    0xc1, 0xe3, /* 1100000111100011 */
    0xc1, 0xb3, /* 1100000110110011 */
    0xc1, 0x83, /* 1100000110000011 */
    0xcf, 0xf3, /* 1100111111110011 */
    0xc0, 0x03, /* 1100000000000011 */
    0xc0, 0x03, /* 1100000000000011 */
    0xff, 0xff, /* 1111111111111111 */
  }
};

static const uint8_t fonts_en[6][16] = {
  {
    /* 72 0x48 'H' */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0xc6, /* 11000110 */
    0xc6, /* 11000110 */
    0xc6, /* 11000110 */
    0xc6, /* 11000110 */
    0xfe, /* 11111110 */
    0xc6, /* 11000110 */
    0xc6, /* 11000110 */
    0xc6, /* 11000110 */
    0xc6, /* 11000110 */
    0xc6, /* 11000110 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
  },
  {
    /* 101 0x65 'e' */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x7c, /* 01111100 */
    0xc6, /* 11000110 */
    0xfe, /* 11111110 */
    0xc0, /* 11000000 */
    0xc0, /* 11000000 */
    0xc6, /* 11000110 */
    0x7c, /* 01111100 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
  },
  {
    /* 108 0x6c 'l' */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x38, /* 00111000 */
    0x18, /* 00011000 */
    0x18, /* 00011000 */
    0x18, /* 00011000 */
    0x18, /* 00011000 */
    0x18, /* 00011000 */
    0x18, /* 00011000 */
    0x18, /* 00011000 */
    0x18, /* 00011000 */
    0x3c, /* 00111100 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
  },
  {
    /* 111 0x6f 'o' */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x7c, /* 01111100 */
    0xc6, /* 11000110 */
    0xc6, /* 11000110 */
    0xc6, /* 11000110 */
    0xc6, /* 11000110 */
    0xc6, /* 11000110 */
    0x7c, /* 01111100 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
  },
  {
    /* 44 0x2c ',' */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x18, /* 00011000 */
    0x18, /* 00011000 */
    0x18, /* 00011000 */
    0x30, /* 00110000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
  },
  {
    /* 46 0x2e '.' */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x18, /* 00011000 */
    0x18, /* 00011000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
    0x00, /* 00000000 */
  }
};

struct font_info {
  uint16_t numglyph;
  uint16_t bytesperglyph;
  uint16_t height;
  uint16_t width;
  uint8_t const *font_data;
};

static const struct font_info zh_info = {
  .numglyph = 2,
  .bytesperglyph = 32,
  .height = 16,
  .width = 16,
  .font_data = (uint8_t const *)fonts_zh,
};

static const struct font_info en_info = {
  .numglyph = 6,
  .bytesperglyph = 16,
  .height = 16,
  .width = 8,
  .font_data = (uint8_t const *)fonts_en,
};

static
uint8_t const *
get_glyph(struct font_info const *font_info, uint16_t index) {
  return font_info->font_data + font_info->bytesperglyph * index;
}

static
void
draw_glyph(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t const *glyph, uint32_t color) {
  for (uint16_t i=y; i<y+height; ++i) {
    for (uint16_t j=0; j<width; j=(uint16_t)(j+8)) {
      uint8_t bits = *glyph;
      for (uint16_t k=0; (k < 8) && ((j+k) < width); ++k)
        if ((bits<<k)&0x80)
          fbbase[fb_info.xres * i + x + j + k] = color;
      glyph += 1;
    }
  }
}

static
void
draw_en(uint16_t x, uint16_t y, uint16_t index, uint32_t color) {
  uint8_t const *glyph = get_glyph(&en_info, index);
  draw_glyph(x, y, en_info.width, en_info.height, glyph, color);
}

static
void
draw_zh(uint16_t x, uint16_t y, uint16_t index, uint32_t color) {
  uint8_t const *glyph = get_glyph(&zh_info, index);
  draw_glyph(x, y, zh_info.width, zh_info.height, glyph, color);
}

int
main(void) {
  char *m = (char *)0x4000000;
  shm_map("shm-1", m);

  fb_get_info(&fb_info);
  fb_map(fbbase);
  uint32_t color[] = {AQUA, FUCHSIA, YELLOW};

  for(;;) {
    for (size_t i=0; i<sizeof(color)/sizeof(color[0]); ++i) {
      uint32_t c = color[i];
      // H
      draw_en(10, 2, 0, c);
      // e
      draw_en(20, 2, 1, c);
      // l
      draw_en(30, 2, 2, c);
      // l
      draw_en(40, 2, 2, c);
      // o
      draw_en(50, 2, 3, c);
      // ,
      draw_en(60, 2, 4, c);
      //zhong
      draw_zh(70, 2, 0, c);
      //guo
      draw_zh(90, 2, 1, c);
      // .
      draw_en(110, 2, 5, c);

      print(*m);
      timer_sleep(1000);
    }
  }
}
