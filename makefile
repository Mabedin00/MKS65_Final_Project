ifeq ($(DEBUG), true)
  CC = gcc -g -c
else
  CC = gcc -c
endif

all: main.o play_song.o server.o networking.o
	gcc -o program main.o play_song.o server.o networking.o

main.o: main.c play_song.h server.h
	$(CC) main.c

play_song.o: play_song.c play_song.h
	$(CC) play_song.c

server.o: server.c server.h networking.h
	$(CC) server.c

networking.o: networking.c networking.h
	$(CC) networking.c

run:
	./program

clean:
	rm *.o
	rm *~
	rm program
