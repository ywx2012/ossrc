DIR := $(lastword $(MAKEFILE_LIST:/Makefile=))
CFLAGS := $(CFLAGS) -I$(DIR)/include/uapi

-include common.mk
$(DIR)_TARGETS := $(BUILDDIR)/kernel.bin
TARGETS := $(TARGETS) $($(DIR)_TARGETS)

KERNEL_CFLAGS := -ffreestanding -std=c11 -I$(DIR)/include -I$(DIR)/include/uapi -fno-pic -mcmodel=large -fno-stack-protector -fno-builtin
KERNEL_LDFLAGS := -nostdlib -Tkernel/linker.ld

$(BUILDDIR)/kernel.bin: $($(DIR)_OBJS) | makedirs
	$(V)ld $(KERNEL_LDFLAGS) -o $@ $^

$(BUILDDIR)/$(DIR)/%.o: $(DIR)/src/%.S | makedirs
	$(V)gcc $(KERNEL_CFLAGS) -c -o $@ $^

$(BUILDDIR)/$(DIR)/%.o: $(DIR)/src/%.c | makedirs
	$(V)gcc $(KERNEL_CFLAGS) -c -o $@ $^

$(BUILDDIR)/$(DIR)/%.d: $(DIR)/src/%.S | makedirs
	$(V)gcc -MM $(KERNEL_CFLAGS) -c -o $@ $^

$(BUILDDIR)/$(DIR)/%.d: $(DIR)/src/%.c | makedirs
	$(V)gcc -MM $(KERNEL_CFLAGS) -c -o $@ $^
