#pragma once

#define PIT_FREQ 1193181
#define PIT_MODE_RATEGEN 0x04

void pit_set_mode(unsigned char channel, unsigned char mode);
void pit_set_count(unsigned char channel, unsigned short count);
