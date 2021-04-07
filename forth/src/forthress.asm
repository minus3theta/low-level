%include "macro.inc"
%include "lib.inc"

%define pc r15
%define w r14
%define rstack r13

section .text

%include "words.inc"

global _start
_start:
  sub rsp, 256
  mov rdi, rsp
  mov rsi, 256
  call read_word
  mov rdi, rax
  call find_word
  lea rdi, [rax+8]
  call print_string

  mov rax, 60
  mov rdi, 0
  syscall

next:

; rdi: word name string
find_word:
  mov rsi, head
.loop:
  test rsi, rsi
  jz .not_found
  push rsi
  push rdi
  add rsi, 8
  call string_equals
  pop rdi
  pop rsi
  test rax, rax
  jnz .found
  mov rsi, [rsi]
  jmp .loop

.found:
  mov rax, rsi
  ret

.not_found:
  xor rax, rax
  ret
