ifeq ($(VERBOSE), 1)
  V=
  Q=
else
  V=@echo GEN $@;
  Q=@
endif

.DEFAULT_GOAL := all

BUILDDIR ?= build
SUBDIRS := $(wildcard */Makefile)
TARGETS := $(BUILDDIR)/initrd.bin
CFLAGS := -fcf-protection=none -mgeneral-regs-only -fno-pic -fno-stack-protector -ffreestanding -std=c11
USER_CFLAGS := $(CFLAGS)
KERNEL_CFLAGS := $(CFLAGS) -mno-red-zone -mlarge-data-threshold=2097152

-include $(SUBDIRS)

$(BUILDDIR)/initrd.bin: $(app_TARGETS) | makedirs
	$(V)printf '%s\n' $(notdir $^) | cpio -D $(dir $<) -ov --format=newc > $@

DEPS := $(OBJS:%.o=%.d)

all: $(TARGETS)

libs: $(LIBS)

makedirs:
	$(Q)mkdir -p $(dir $(TARGETS) $(OBJS))

clean:
	rm -f $(TARGETS) $(LIBS) $(OBJS) $(DEPS)

run: $(TARGETS)
	qemu-system-x86_64 -kernel $(BUILDDIR)/kernel.bin -initrd $(BUILDDIR)/initrd.bin

.PRECIOUS: $(OBJS)

-include $(DEPS)
