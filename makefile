all: main.c io.c operations.c utils.c
	gcc main.c io.c utils.c operations.c -o app -g -W -pedantic

main: main.c
	gcc main.c -c -g -W -pedantic

io: io.c
	gcc io.c -c -g -W -pedantic

operations: operations.c
	gcc operations.c -c -g -W -pedantic

utils: utils.c
	gcc utils.c -c -g -W -pedantic