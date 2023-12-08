all: main.c io.c
	gcc main.c io.c -o app -g -W -pedantic

funcs: funcs.c
	gcc funcs.c -c -g -W -pedantic

io: io.c
	gcc io.c -c -g -W -pedantic