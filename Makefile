kernel.bin: build system.bin app/app1.bin app/app2.bin
	./build

CFLAGS = -std=c11 -I. -fno-pic -mcmodel=kernel -fno-stack-protector -fno-builtin
SRCS = main.c $(wildcard mm/*.c) $(wildcard lib/*.c) $(wildcard kernel/*.c) $(wildcard ipc/*.c) \
			 $(wildcard drivers/*.c)
OBJS = boot16.o head64.o kernel/handler.o $(SRCS:.c=.o)

system.bin: $(OBJS)
	ld -Ttext=0xffffffff800f9000 $(OBJS) -o system.elf
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
app/libdraw/draw.o: app/libdraw/draw.c
	gcc -I. -c -o $@ app/libdraw/draw.c

app/libdraw/fonts.o: app/libdraw/fonts.c
	gcc -I. -c -o $@ app/libdraw/fonts.c

app/libdraw/libdraw.o: app/libdraw/draw.o app/libdraw/fonts.o
	ld -r -o $@ $^

# app
app/app1.o: app/app1.c
	gcc -I. -c -o $@ $<

app/app1.bin: app/libc/start.o app/app1.o app/libc/libc.o app/libdraw/libdraw.o
	ld -Ttext=0x100000 $^ -o app/app1.elf
	objcopy -R .note.* -O binary app/app1.elf $@

app/app2.o: app/app2.c
	gcc -fno-stack-protector -I. -c -o $@ $<

app/app2.bin: app/libc/start.o app/app2.o app/libc/libc.o app/libdraw/libdraw.o
	ld -Ttext=0x100000 $^ -o app/app2.elf
	objcopy -R .note.* -O binary app/app2.elf $@

# build tool
build: build.c
	gcc $< -o $@

.PHONY: clean run

run: kernel.bin
	qemu-system-x86_64 -kernel ./kernel.bin

clean:
	find -name "*.o" -o -name "*.elf" -o -name "*.bin" | xargs rm -f
	rm -f build .depend
