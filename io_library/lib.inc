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


; rdi, rsi: string pointers
; returns: 1 if equal, 0 otherwise
string_equals:
  mov al, byte [rdi]
  cmp al, byte [rsi]
  jne .diff

  inc rdi
  inc rsi

  test al, al ; reached end of string
  jnz string_equals

  ; same
  mov rax, 1
  ret

.diff:
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
  ; rdi: buf
  ; rsi: buf size

  push r14
  push r15
  xor r14, r14 ; r14: word len
  mov r15, rsi ; r15: buf size
  dec r15

.skip_ws:
  push rdi
  call read_char
  ; rax: char
  pop rdi

  cmp al, 0
  je .end
  cmp al, 0x20
  je .skip_ws
  cmp al, 0x09
  je .skip_ws
  cmp al, 0x0a
  je .skip_ws
  cmp al, 0x0d
  je .skip_ws

.loop:
  mov [rdi + r14], al
  inc r14

  push rdi
  call read_char
  ; rax: char
  pop rdi
  cmp al, 0
  je .end
  cmp al, 0x20
  je .end
  cmp al, 0x09
  je .end
  cmp al, 0x0a
  je .end
  cmp al, 0x0d
  je .end

  cmp r14, r15
  jne .loop

  ; buffer overflow
  xor rax, rax
  pop r15
  pop r14
  ret

.end:
  mov [rdi + r14], byte 0
  mov rax, rdi
  mov rdx, r14
  pop r15
  pop r14
  ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
  xor rax, rax ; initialize number
  xor rcx, rcx ; initialize length
  mov r8, 10

.loop:
  movzx r9, byte [rdi + rcx]
  cmp r9b, '0'
  jb .end
  cmp r9b, '9'
  ja .end

  inc rcx
  and r9b, 0x0f
  mul r8
  add rax, r9
  jmp .loop

.end:
  mov rdx, rcx
  ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
  movzx r9, byte [rdi]
  cmp r9b, '-'
  ; positive
  jne parse_uint

  ; negative
  inc rdi
  call parse_uint
  ; rax: abs, rdx: length - 1
  neg rax
  inc rdx
  ret


string_copy:
  ret
