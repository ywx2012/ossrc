#include <user/syscall.h>
#include <user/timer.h>
#include <user/shm.h>
#include <kernel/task.h>

int
system_call(uintptr_t edi, uintptr_t esi, uintptr_t ebx) {
  switch (ebx) {
  case SYS_TIMER_SLEEP:
    return timer_sleep(edi);
  case SYS_SHM_MAP:
    return shm_map((char const *)edi, (void *)esi);
  case SYS_SHM_GET_INFO:
    return shm_get_info((char const *)edi, (struct shm_info *)esi);
  default:
    return -1;
  }
}
