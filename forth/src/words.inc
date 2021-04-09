%include "macro.inc"

native '+', plus
  pop rax
  add [rsp], rax
  jmp next

native '-', sub
  pop rax
  sub [rsp], rax
  jmp next

native '*', mul
  pop rax
  mul qword [rsp]
  mov [rsp], rax
  jmp next

native '/', div
  pop rdi
  pop rax
  xor rdx, rdx
  div rdi
  push rax
  jmp next

native 'swap', swap
  pop rax
  pop rdi
  push rax
  push rdi
  jmp next

native '=', eq
  pop rax
  pop rdi
  xor rsi, rsi
  cmp rdi, rax
  sete sil
  push rsi
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

native '.S', dot_s
  mov rsi, rsp
.loop:
  cmp [stack_base], rsi
  je .end
  push rsi
  mov rdi, [rsi]
  call print_int
  mov rdi, 10
  call print_char
  pop rsi
  add rsi, 8
  jmp .loop

.end:
  jmp next

native 'emit', emit
  pop rdi
  call print_int
  mov rdi, 10
  call print_char
  jmp next
