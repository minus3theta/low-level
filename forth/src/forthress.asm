%include "macro.inc"
%include "lib.inc"

%define pc r15
%define w r14
%define rstack r13

section .rodata
msg_unknown_word: db "Error: unknown word", 10, 0

section .text

%include "words.inc"

global _start

section .text

; rdi: word name string
find_word:
  mov rsi, [last_word]
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


; rdi: start address of word header
; returns: start address of XT
cfa:
  add rdi, 8
  push rdi
  call string_length
  pop rdi
  lea rax, [rdi+rax+2]
  ret


next:
  mov w, [pc]
  add pc, 8
  jmp [w]


docol:
  sub rstack, 8
  mov [rstack], pc
  add w, 8
  mov pc, w
  jmp next


section .data
program_stub: dq 0
xt_interpreter: dq .interpreter
.interpreter: dq interpreter_loop

stack_base: dq 0

state: dq 0
here: dq user_dict
last_word: dq head

section .bss
resq 1023
rstack_start: resq 1
user_mem: resq 65536
user_dict: resq 65536

input_buf: resb 1024

section .text

_start:
  mov [stack_base], rsp
  mov rstack, rstack_start

interpreter_loop:
  mov rdi, input_buf
  mov rsi, 1024
  call read_word
  test rdx, rdx
  jz .word_empty

  push rax
  mov rdi, rax
  call find_word
  test rax, rax
  jz .not_found

  ; word found
  add rsp, 8
  mov rdi, rax
  call cfa

  ; execute if in interpreter mode or immediate word
  test qword [state], -1
  jz .execute
  test byte [rax-1], -1
  jnz .execute

  ; compile
  mov rdi, [here]
  mov [rdi], rax
  add qword [here], 8
  jmp interpreter_loop

.execute:
  mov [program_stub], rax
  mov pc, program_stub
  jmp next

.not_found:
  pop rdi
  call parse_int
  test rdx, rdx
  jz .unknown_word

  ; int literal
  test qword [state], -1
  jz .push_imm
  mov rdi, [here]
  mov qword [rdi], xt_lit
  mov [rdi+8], rax
  add qword [here], 16
  jmp interpreter_loop

.push_imm:
  push rax
  jmp interpreter_loop

.word_empty:
  mov rax, 60
  xor rdi, rdi
  syscall

.unknown_word:
  mov rdi, msg_unknown_word
  call print_string_err
  mov rax, 60
  mov rdi, 1
  syscall
