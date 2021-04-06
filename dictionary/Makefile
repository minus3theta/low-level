all: lib.o

lib.o: lib.inc
	nasm -f elf64 $< -o $@

%.o: %.asm
	nasm -f elf64 $< -o $@

%: %.o
	ld -o $@ $<
