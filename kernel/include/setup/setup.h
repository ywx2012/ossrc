#pragma once
#include <asm/bootparam.h>
#include <asm/e820.h>
#include <x86/segment.h>

extern struct setup_header setup_header;
extern struct e820map e820map;
extern struct dtr gdtr;
extern uint8_t font_data[128*16];

void setup_start(void);
