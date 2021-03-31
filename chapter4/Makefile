%.o: %.asm
	nasm -felf64 $< -o $@

%: %.o
	ld -o $@ $<
