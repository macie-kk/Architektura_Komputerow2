all: hello writeText #compareText


hello.o: hello.s
	as -o hello.o hello.s

hello: hello.o
	ld -o hello hello.o

writeText.o: writeText.s
	as -o writeText.o writeText.s

writeText: writeText.o
	ld -o writeText writeText.o

compareText.o: compareText.s
	as -o compareText.o compareText.s

compareText: compareText.o
	ld -o compareText compareText.o
