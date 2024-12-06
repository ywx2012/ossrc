#include <user/syscall.h>
#include <user/timer.h>
#include <user/fb.h>
#include <user/shm.h>
#include <kernel/task.h>

int
system_call(uintptr_t rdi, uintptr_t rsi, uintptr_t rdx) {
  switch (rdx) {
  case SYS_TIMER_SLEEP:
    return timer_sleep(rdi);
  case SYS_SHM_MAP:
    return shm_map((char const *)rdi, (void *)rsi);
  case SYS_FB_MAP:
    return fb_map((void *)rdi);
  case SYS_FB_GET_INFO:
    return fb_get_info((struct fb_info *)rdi);
  default:
    return -1;
  }
}
