DIR := $(lastword $(MAKEFILE_LIST:/Makefile=))
USER_LDFLAGS := $(USER_LDFLAGS) -static -nostdlib -T$(DIR)/linker.ld

-include lib.mk
