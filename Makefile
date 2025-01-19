ifeq ($(VERBOSE), 1)
  V=
  Q=
else
  V=@echo GEN $@;
  Q=@
endif

.DEFAULT_GOAL := all

kernel_TARGETS := $(wildcard kernel/.cod/dev.x86_64/bin/*.bin)
app_TARGETS := $(wildcard app/.cod/dev.x86_64/bin/*.elf)

kernel.bin: $(kernel_TARGETS)
	cp $< $@

initrd.bin: $(app_TARGETS)
	$(V)printf '%s\n' $(notdir $^) | cpio -D $(dir $<) -ov --format=newc > $@

all: kernel.bin initrd.bin

clean:
	rm -f kernel.bin initrd.bin
