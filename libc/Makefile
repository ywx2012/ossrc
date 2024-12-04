DIR := $(lastword $(MAKEFILE_LIST:/Makefile=))
LDFLAGS := $(LDFLAGS) -static -nostdlib -T$(DIR)/linker.ld

-include lib.mk
