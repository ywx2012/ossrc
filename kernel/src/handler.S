
// Copyright (c) 2023 Wang Baisheng <baisheng_wang@163.com>, Wang Shenghan. All Rights Reserved.

.text
.code64
.globl timer_handler
.globl pf_handler
.globl kb_handler
.globl system_call

.macro SAVE_CONTEXT save_rax = 1
  pushq %rdi
  pushq %rsi
  pushq %rdx
  pushq %rcx
  .if \save_rax
  pushq %rax
  .endif
  pushq %r8
  pushq %r9
  pushq %r10
  pushq %r11
.endm

.macro RESTORE_CONTEXT rstore_rax = 1
  popq %r11
  popq %r10
  popq %r9
  popq %r8
  .if \rstore_rax
  popq %rax
  .endif
  popq %rcx
  popq %rdx
  popq %rsi
  popq %rdi
.endm

system_call:
  movabs $tss, %rbx
  mov %rsp, 20(%rbx)
  mov 4(%rbx), %rsp

  pushq 20(%rbx)
  SAVE_CONTEXT 0

  movabs $syscall_table, %rbx
  call *(%rbx, %rax, 8)

  RESTORE_CONTEXT 0
  pop %rsp

  sysretq
