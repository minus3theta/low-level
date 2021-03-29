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
  mov rdi, 10
  jmp print_char


print_int:
  test rdi, rdi
  jns print_uint

  push rdi
  mov rdi, '-'
  call print_char
  pop rdi

  neg rdi
  ; go to print_uint

print_uint:
  ; rdi: 8-bytes unsigned integer

  mov rax, rdi ; rax <- integer to print
  mov rdi, rsp ; rdi <- end of buffer
  ; allocate 24 bytes buffer
  push 0
  sub rsp, 16

  ;      top
  ; +-----------+ <- rsp
  ; |           |
  ; +-----------+
  ; |           |
  ; +-----------+
  ; |         0 |
  ; +-----------+ <- rdi
  ;     bottom

  dec rdi ; last character is '\0'
  mov r8, 10 ; r8 is divisor

.loop:
  xor rdx, rdx ; clear rdx
  div r8 ; rdx = rax % 10, rax /= 10
  or rdx, 0x30 ; convert digit to char
  dec rdi ; move buffer head to front
  mov [rdi], dl ; save character to buffer
  test rax, rax
  jnz .loop ; loop if rax != 0

  ; rdi: buffer (null terminated)
  call print_string

  ; release buffer
  add rsp, 24
  ret


string_equals:
  xor rax, rax
  ret


read_char:
  push 0

  mov rax, 0 ; read syscall
  mov rdi, 0 ; fd = stdin (0)
  mov rsi, rsp ; buf
  mov rdx, 1 ; count
  syscall

  pop rax
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
