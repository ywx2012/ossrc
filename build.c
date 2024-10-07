
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

#include <unistd.h>
#include <fcntl.h>

#define BASE (0x100000 - 0x7000)

int main() {
  int fd, fd_kernel;
  int c;
  char buf[512];

  fd_kernel = open("kernel.bin", O_WRONLY | O_CREAT, 0666);

  // boot16.bin
  fd = open("boot16.bin", O_RDONLY);
  while (1) {
    c = read(fd, buf, 512);
    if (c > 0) {
      write(fd_kernel, buf, c);
    } else {
      break;
    }
  };
  close(fd);

  // system.bin
  lseek(fd_kernel, 0x100000 - BASE, SEEK_SET);
  fd = open("system.bin", O_RDONLY);
  while (1) {
    c = read(fd, buf, 512);
    if (c > 0) {
      write(fd_kernel, buf, c);
    } else {
      break;
    }
  };
  close(fd);

  // app1
  lseek(fd_kernel, 0x1000000 - BASE, SEEK_SET);
  fd = open("app/app1.bin", O_RDONLY);
  while (1) {
    c = read(fd, buf, 512);
    if (c > 0) {
      write(fd_kernel, buf, c);
    } else {
      break;
    }
  };
  close(fd);

  // app2
  lseek(fd_kernel, 0x2000000 - BASE, SEEK_SET);
  fd = open("app/app2.bin", O_RDONLY);
  while (1) {
    c = read(fd, buf, 512);
    if (c > 0) {
      write(fd_kernel, buf, c);
    } else {
      break;
    }
  };
  close(fd);

  close(fd_kernel);

  return 0;
}
