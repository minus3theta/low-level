section .text

extern string_equals

; dictionary structure
; - next pointer
; - word
; - description

; rdi: key string
; rsi: dict pointer
; return value: record address
global find_word
find_word:
  test rsi, rsi
  jz .not_found

  push rdi
  push rsi
  add rsi, 8
  call string_equals
  test rax, rax
  jnz .found

  pop rsi
  pop rdi
  mov rsi, [rsi]
  jmp find_word

.found:
  pop rax
  add rsp, 8
  ret

.not_found:
  xor rax, rax
  ret
