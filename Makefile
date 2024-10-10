CFLAGS = -std=c11 -I. -fno-pic -mcmodel=large -fno-stack-protector -fno-builtin
SRCS = main.c $(wildcard mm/*.c) $(wildcard lib/*.c) $(wildcard kernel/*.c) $(wildcard ipc/*.c) \
			 $(wildcard drivers/*.c)
OBJS = boot16.o head64.o kernel/handler.o $(SRCS:.c=.o)
APPS = $(wildcard app/*.c)

kernel.bin: build system.bin $(APPS:%.c=%.bin)
	./build

system.bin: $(OBJS)
	ld -Ttext=0xffff8000000ff000 $(OBJS) -o system.elf
	objcopy -R .note.* -O binary system.elf $@

.depend: $(SRCS)
	@rm -f .depend
	@$(foreach src,$(SRCS), \
		echo -n $(dir $(src)) >> .depend; \
		gcc -I. -MM $(src) >> .depend; \
	)
include .depend

# libc
app/libc/libc.o: app/libc/syscall.o
	ld -r -o $@ $^

# libdraw
app/libdraw/%.o: app/libdraw/%.c
	gcc -I. -c -o $@ $^

app/libdraw/libdraw.o: app/libdraw/draw.o app/libdraw/fonts.o
	ld -r -o $@ $^

# app
app/%.o: app/%.c
	gcc -fno-stack-protector -I. -c -o $@ $<

app/%.bin: app/libc/start.o app/%.o app/libc/libc.o app/libdraw/libdraw.o
	ld -Ttext=0x100000 $^ -o $(@:.bin=.elf)
	objcopy -R .note.* -O binary $(@:.bin=.elf) $@

# build tool
build: build.c
	gcc $< -o $@

.PHONY: clean run
.PRECIOUS: $(APPS:%.c=%.o)

run: kernel.bin
	qemu-system-x86_64 -kernel ./kernel.bin

clean:
	find -name "*.o" -o -name "*.elf" -o -name "*.bin" | xargs rm -f
	rm -f build .depend
