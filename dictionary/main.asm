section .data

%include "words.inc"
msg_not_found: db "word not found", 10

section .text

global _start

extern find_word
extern read_word
extern print_string
extern print_char
extern string_length

_start:
  sub rsp, 256
  mov rdi, rsp
  mov rsi, 256

  call read_word
  ; rax: input string

  mov rdi, rax
  mov rsi, baz
  call find_word

  test rax, rax
  jz .not_found

  ; found
  lea rdi, [rax+8]
  push rdi
  call string_length

  pop rdi
  lea rdi, [rdi+rax+1]
  call print_string

  mov rdi, 10
  call print_char

  jmp .end

.not_found:
  mov rax, 1
  mov rdi, 2
  mov rsi, msg_not_found
  mov rdx, 15
  syscall

.end:
  add rsp, 256

  mov rax, 60
  mov rdi, 0
  syscall
