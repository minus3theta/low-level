# ------------------------------------------------
# Forthress, a Forth dialect
#
# Author: igorjirkov@gmail.com
# Date  : 15-10-2016
#
# ------------------------------------------------

ASM = nasm
FLAGS = -felf64 -g -Isrc/
LINKER = ld

all: bin/forthress bin/itc

bin/forthress: obj/forthress.o obj/lib.o
	$(LINKER) -o $@ $^

obj/forthress.o: src/forthress.asm src/macro.inc src/words.inc src/lib.inc
	$(ASM) $(FLAGS) $< -o $@

obj/lib.o: src/lib.asm src/lib.inc
	$(ASM) $(FLAGS) $< -o $@

obj/itc.o: example/itc.asm src/lib.inc
	$(ASM) $(FLAGS) $< -o $@

bin/itc: obj/itc.o obj/lib.o
	$(LINKER) -o $@ $^

clean:
	$(RM) build/* obj/*

.PHONY: clean
