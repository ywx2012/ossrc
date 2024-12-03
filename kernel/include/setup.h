#pragma once
#include <asm/bootparam.h>
#include <asm/e820.h>
#include <x86/segment.h>
#include <header.h>

extern struct setup_header setup_header;
extern struct e820map e820map;
extern struct dtr GDTR;

void setup_start(void);
