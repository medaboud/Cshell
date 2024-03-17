wish: main.o wish.o util.o inputProcessing.o
	gcc -o wish main.o wish.o util.o inputProcessing.o

inputProcessing.o: inputProcessing.c
	gcc -c inputProcessing.c

util.o: util.c
	gcc -c util.c

wish.o: wish.c
	gcc -c wish.c

main.o: main.c
	gcc -c main.c
