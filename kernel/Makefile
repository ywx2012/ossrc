DIR := $(lastword $(MAKEFILE_LIST:/Makefile=))
INCLUDE_DIRS := $(INCLUDE_DIRS) -I$(DIR)/include/user

-include common.mk
$(DIR)_TARGETS := $(BUILDDIR)/kernel.bin
TARGETS := $(TARGETS) $($(DIR)_TARGETS)

KERNEL_LDFLAGS := $(KERNEL_CFLAGS) -flto -no-pie -nostdlib -Wl,-T$(DIR)/linker.ld

$(BUILDDIR)/kernel.bin: $(DIR)/linker.ld $($(DIR)_OBJS) | makedirs
	$(V)gcc $(KERNEL_LDFLAGS) -o $@ $(filter $(BUILDDIR)/%,$^)

$(BUILDDIR)/$(DIR)/realmode.o: $(DIR)/src/realmode.c | makedirs
	$(V)gcc -MMD $(CFLAGS) $(INCLUDE_DIRS) -c -o $@ $<
	objcopy -I elf32-i386 -O elf64-x86-64 $@

$(BUILDDIR)/$(DIR)/%.o: INCLUDE_DIRS := -I$(DIR)/include
$(BUILDDIR)/$(DIR)/realmode.o: CFLAGS := $(KERNEL_CFLAGS) -m16 -fno-lto
$(BUILDDIR)/$(DIR)/setup.o: CFLAGS := $(KERNEL_CFLAGS) -mcmodel=large -fno-lto
$(BUILDDIR)/$(DIR)/%.o: CFLAGS := $(KERNEL_CFLAGS) -mcmodel=large -flto
