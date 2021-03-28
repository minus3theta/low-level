section .text

string_length:
  xor rax, rax

.loop:
  cmp byte [rdi+rax], 0
  je .end
  inc rax
  jmp .loop

.end:
  ret

print_string:
  push rdi
  call string_length

  mov rdx, rax ; rdx <- string length
  pop rsi ; rsi <- string head
  mov rax, 1 ; write syscall
  mov rdi, 1 ; rdi <- file descriptor
  syscall

  ret


print_char:
  push di
  mov rax, 1
  mov rdi, 1
  mov rsi, rsp
  mov rdx, 1
  syscall

  pop di

  ret

print_newline:
  xor rax, rax
  ret


print_uint:
  xor rax, rax
  ret


print_int:
  xor rax, rax
  ret

string_equals:
  xor rax, rax
  ret


read_char:
  xor rax, rax
  ret

read_word:
  ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
  xor rax, rax
  ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
  xor rax, rax
  ret


string_copy:
  ret
