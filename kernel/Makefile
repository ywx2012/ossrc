DIR := $(lastword $(MAKEFILE_LIST:/Makefile=))
USER_CFLAGS := $(USER_CFLAGS) -I$(DIR)/include/user

-include common.mk
$(DIR)_TARGETS := $(BUILDDIR)/kernel.bin
TARGETS := $(TARGETS) $($(DIR)_TARGETS)

KERNEL_CFLAGS := $(KERNEL_CFLAGS) -I$(DIR)/include
KERNEL_LDFLAGS := -nostdlib -T$(DIR)/linker.ld

$(BUILDDIR)/kernel.bin: $(DIR)/linker.ld $($(DIR)_OBJS) | makedirs
	$(V)ld $(KERNEL_LDFLAGS) -o $@ $(filter $(BUILDDIR)/%,$^)

$(BUILDDIR)/$(DIR)/%.o: $(DIR)/src/%.S | makedirs
	$(V)gcc $(KERNEL_CFLAGS) -mcmodel=large -D__ASSEMBLY__ -c -o $@ $<

$(BUILDDIR)/$(DIR)/realmode.o: $(DIR)/src/realmode.c | makedirs
	$(V)gcc $(KERNEL_CFLAGS) -m16 -c -o $@ $<
	objcopy -I elf32-i386 -O elf64-x86-64 $@

$(BUILDDIR)/$(DIR)/%.o: $(DIR)/src/%.c | makedirs
	$(V)gcc $(KERNEL_CFLAGS) -mcmodel=large -c -o $@ $<

$(BUILDDIR)/$(DIR)/%.d: $(DIR)/src/%.S | makedirs
	$(V)gcc -MM -MT ${@:%.d=%.o} $(KERNEL_CFLAGS) -mcmodel=large -D__ASSEMBLY__ -c -o $@ $<

$(BUILDDIR)/$(DIR)/setup.d: $(DIR)/src/setup.c | makedirs
	$(V)gcc -MM -MT ${@:%.d=%.o} $(KERNEL_CFLAGS) -m16 -c -o $@ $<

$(BUILDDIR)/$(DIR)/%.d: $(DIR)/src/%.c | makedirs
	$(V)gcc -MM -MT ${@:%.d=%.o} $(KERNEL_CFLAGS) -mcmodel=large -c -o $@ $<
