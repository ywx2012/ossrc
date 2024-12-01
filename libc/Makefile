DIR := $(lastword $(MAKEFILE_LIST:/Makefile=))
CFLAGS := $(CFLAGS) -ffreestanding -fno-pic -fno-stack-protector -fno-builtin
LDFLAGS := $(LDFLAGS) -static -nostdlib -T$(DIR)/linker.ld

-include lib.mk
