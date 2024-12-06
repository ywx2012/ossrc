#include <syscall.h>
#include <timer.h>
#include <shm.h>

int
timer_sleep(uintptr_t ms) {
  return syscall(SYS_TIMER_SLEEP, ms, 0);
}

int
shm_map(char const *name, void *ptr) {
  return syscall(SYS_SHM_MAP, (uintptr_t)name, (uintptr_t)ptr);
}

int
shm_get_info(char const *name, struct shm_info *shm_info) {
  return syscall(SYS_SHM_GET_INFO, (uintptr_t)name, (uintptr_t)shm_info);
}
