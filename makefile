ifeq ($(DEBUG), true)
  CC = gcc -g -c
else
  CC = gcc -c
endif

all: main.o executes.o
	gcc -o program main.o executes.o

main.o: main.c executes.h
	$(CC) main.c

executes.o: executes.c executes.h
	$(CC) executes.c

run:
	./program

clean:
	rm program
	rm *.o
