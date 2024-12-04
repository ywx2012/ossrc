ifeq ($(VERBOSE), 1)
  V=
  Q=
else
  V=@echo GEN $@;
  Q=@
endif

.DEFAULT_GOAL := all

DETECT_CFLAGS := \
	$(shell gcc --help=c | grep -q fstrict-flex-arrays && echo '-fstrict-flex-arrays -Wstrict-flex-arrays') \
	$(shell gcc --help=c | grep -q Wflex-array-member-not-at-end && echo '-Wflex-array-member-not-at-end') \
	$(shell gcc --help=c | grep -q Winvalid-utf8 && echo '-Winvalid-utf8') \
	$(shell gcc --help=c | grep -q Wmissing-variable-declarations && echo '-Wmissing-variable-declarations') \
	$(shell gcc --help=c | grep -q Wuseless-cast && echo '-Wuseless-cast') \
	$(shell gcc --help=common | grep -q Wtrivial-auto-var-init && echo '-Wtrivial-auto-var-init') \
	$(shell gcc --help=common | grep -q Wuse-after-free && echo '-Wuse-after-free=3')

BUILDDIR ?= build
SUBDIRS := $(wildcard */Makefile)
TARGETS := $(BUILDDIR)/initrd.bin
CFLAGS := -mgeneral-regs-only -fno-stack-protector -ffreestanding -std=c11 \
	-Werror -Wall -Wextra \
	-pedantic -pedantic-errors -Wno-long-long \
	-Waggregate-return \
	-Walloc-zero \
	-Walloca \
	-Warith-conversion \
	-Wbad-function-cast \
	-Wcast-align=strict \
	-Wcast-qual \
	-Wconversion \
	-Wdate-time \
	-Wdisabled-optimization \
	-Wdouble-promotion \
	-Wduplicated-branches \
	-Wduplicated-cond \
	-Wfloat-equal \
	-Wformat=2 \
	-Wformat-signedness \
	-Winit-self \
	-Winline \
	-Winvalid-pch \
	-Wjump-misses-init \
	-Wlogical-op \
	-Wmissing-declarations \
	-Wmissing-include-dirs \
	-Wmissing-prototypes \
	-Wmultichar \
	-Wnested-externs \
	-Wnull-dereference \
	-Wpacked \
	-Wpadded \
	-Wredundant-decls \
	-Wshadow \
	-Wstack-protector \
	-Wstrict-prototypes \
	-Wswitch-default \
	-Wswitch-enum \
	-Wtrampolines \
	-Wundef \
	-Wunsuffixed-float-constants \
	-Wunused-macros \
	-Wvector-operation-performance \
	-Wwrite-strings \
	-Wattribute-alias=2 \
	-Wimplicit-fallthrough=5 \
	-Wnormalized=nfkc \
	-Wshift-overflow=2 \
	-Wstrict-overflow=5 \
	-Wunused-const-variable=2 \
	$(DETECT_CFLAGS)

KERNEL_CFLAGS = $(CFLAGS) -fno-pie -fcf-protection=none -mno-red-zone

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
