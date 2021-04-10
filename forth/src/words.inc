%include "macro.inc"

; std

native 'exit', exit
  mov pc, [rstack]
  add rstack, 8
  jmp next

; arithmetic

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
  dq xt_exit

; logical

native 'and', and
  pop rax
  pop rdi
  test rax, rax
  jz .false
  xor rsi, rsi
  test rdi, rdi
  setnz sil
  push rsi
  jmp next

.false:
  push qword 0
  jmp next

native 'not', not
  pop rax
  xor rsi, rsi
  test rax, rax
  setz sil
  push rsi
  jmp next

colon 'or', or
  dq xt_not
  dq xt_swap
  dq xt_not
  dq xt_and
  dq xt_not
  dq xt_exit

; stack operation

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

native 'rot', rot
  pop rax
  pop rdi
  pop rsi
  push rdi
  push rax
  push rsi
  jmp next

native 'swap', swap
  pop rax
  pop rdi
  push rax
  push rdi
  jmp next

native 'dup', dup
  mov rax, [rsp]
  push rax
  jmp next

native 'drop', drop
  add rsp, 8
  jmp next

native '.', put
  pop rdi
  call print_int
  mov rdi, 10
  call print_char
  jmp next

; IO

native 'key', key
  call read_char
  push rax
  jmp next

native 'emit', emit
  pop rdi
  call print_int
  mov rdi, 10
  call print_char
  jmp next

native 'number', number
  sub rsp, 256
  mov rdi, rsp
  mov rsi, 256
  call read_word
  test rdx, rdx
  jz .empty
  mov rdi, rax
  call parse_int
  add rsp, 256
  push rax
  jmp next

.empty:
  add rsp, 256
  push qword 0
  jmp next

; memory operation

native 'mem', mem
  push user_mem
  jmp next

native '!', store
  pop rax
  pop rdi
  mov [rdi], rax
  jmp next

native 'c!', store_c
  pop rax
  pop rdi
  mov [rdi], al
  jmp next

native '@', load
  pop rdx
  mov rax, [rdx]
  push rax
  jmp next

native 'c@', load_c
  pop rdx
  movzx rax, byte [rdx]
  push rax
  jmp next

; word definition

native ':', colon
  mov rdi, input_buf
  mov rsi, 1024
  call read_word

  ; header
  ; link
  mov r8, [here]
  push r8
  mov rsi, [last_word]
  mov [r8], rsi
  add r8, 8
  ; name
  push r8
  mov rdi, rax
  mov rsi, r8
  mov rdx, 1024
  call string_copy
  mov rdi, rax
  call string_length
  pop r8
  lea r8, [r8+rax+1]
  ; flag
  mov [r8], byte 0
  inc r8
  ; docol
  mov qword [r8], docol
  add r8, 8

  mov [here], r8

  ; last_word
  pop qword [last_word]
  mov qword [state], 1

  jmp next

native ';', semicolon, 1
  mov r8, [here]
  mov qword [r8], xt_exit
  add qword [here], 8
  mov qword [state], 0
  jmp next
