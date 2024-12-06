#include <user/syscall.h>
#include <user/timer.h>
#include <user/shm.h>
#include <kernel/task.h>

int
system_call(uintptr_t rdi, uintptr_t rsi, uintptr_t rdx) {
  switch (rdx) {
  case SYS_TIMER_SLEEP:
    return timer_sleep(rdi);
  case SYS_SHM_MAP:
    return shm_map((char const *)rdi, (void *)rsi);
  case SYS_SHM_GET_INFO:
    return shm_get_info((char const *)rdi, (struct shm_info *)rsi);
  default:
    return -1;
  }
}
