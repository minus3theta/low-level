%include "macro.inc"

native '+', plus
  pop rax
  add [rsp], rax
  jmp next

native '-', sub
  pop rax
  sub [rsp], rax
  jmp next

native 'swap', swap
  pop rax
  pop rdi
  push rax
  push rdi
  jmp next

native '<', less
  pop rax
  pop rdi
  xor rsi, rsi
  cmp rdi, rax
  setl sil
  push rsi
  jmp next

colon '>', greater
  dq xt_swap
  dq xt_less
  dq exit
