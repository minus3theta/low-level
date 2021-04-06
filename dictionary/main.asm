section .data


section .text

global _start

extern find_word
extern read_word
extern print_string

_start:
  sub rsp, 256
  mov rdi, rsp
  mov rsi, 256

  call read_word

  mov rdi, rax
  call print_string

  add rsp, 256

  mov rax, 60
  mov rdi, 0
  syscall
