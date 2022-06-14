all: dodawanie

dodawanie:    dodawanie.o
	ld dodawanie.o -o dodawanie

dodawanie.o:	dodawanie.s
	as --gstabs dodawanie.s -o dodawanie.o

