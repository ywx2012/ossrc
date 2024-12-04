#include <uapi/syscall.h>
#include <uapi/timer.h>
#include <uapi/fb.h>
#include <uapi/shm.h>

int
system_call(uintptr_t rdi, uintptr_t rsi, uintptr_t rdx) {
  switch (rdx) {
  case SYS_TIMER_SLEEP:
    return timer_sleep(rdi);
  case SYS_SHM_MAP:
    return shm_map((char const *)rdi, rsi);
  case SYS_FB_MAP:
    return fb_map(rdi);
  case SYS_FB_GET_INFO:
    return fb_get_info((struct fb_info *)rdi);
  default:
    return -1;
  }
}
