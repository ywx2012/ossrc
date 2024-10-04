
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
  mov %rsp, tss + 20
  mov tss + 4, %rsp

  pushq tss + 20
  SAVE_CONTEXT 0

  call *syscall_table(, %rax, 8)

  RESTORE_CONTEXT 0
  pop %rsp

  sysretq

timer_handler:
  SAVE_CONTEXT

  movb $0x20,%al
  outb %al,$0x20

  call do_timer

  RESTORE_CONTEXT
  iretq

pf_handler:
  SAVE_CONTEXT

  mov %cr2, %rdi
  call do_page_fault

  RESTORE_CONTEXT
  add $8, %rsp
  iretq

kb_handler:
  SAVE_CONTEXT

  call process_kb

  movb $0x20,%al
  outb %al,$0x20

  RESTORE_CONTEXT
  iretq

