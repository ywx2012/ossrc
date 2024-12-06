#include <syscall.h>
#include <timer.h>
#include <shm.h>
#include <fb.h>

int
timer_sleep(uintptr_t ms) {
  return syscall(SYS_TIMER_SLEEP, ms, 0);
}

int
shm_map(char const *name, void *ptr) {
  return syscall(SYS_SHM_MAP, (uintptr_t)name, (uintptr_t)ptr);
}

int
fb_map(void *ptr) {
  return syscall(SYS_FB_MAP, (uintptr_t)ptr, 0);
}

int
fb_get_info(struct fb_info *fb_info) {
  return syscall(SYS_FB_GET_INFO, (uintptr_t)fb_info, 0);
}
