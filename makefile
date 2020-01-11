ifeq ($(DEBUG), true)
  CC = gcc -g -c
else
  CC = gcc -c
endif

all: main.o play_song.o
	gcc -o program main.o play_song.o

main.o: main.c play_song.h
	$(CC) main.c

play_song.o: play_song.c play_song.h
	$(CC) play_song.c

run:
	./program

clean:
	rm *.o
	rm *~
	rm program
