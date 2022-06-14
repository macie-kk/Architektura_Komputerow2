all: wyjatki

wyjatki.o: wyjatki.s
	as -g --32 -o wyjatki.o wyjatki.s

wyjatki: wyjatki.o
	ld -m elf_i386 -o wyjatki wyjatki.o
