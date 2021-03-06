ifeq ($(DEBUG), true)
  CC = gcc -g -c
else
  CC = gcc -c
endif

all: main.o play_song.o server.o networking.o client.o queue.o library.o
	gcc -o program main.o play_song.o server.o networking.o client.o queue.o library.o `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

main.o: main.c play_song.h server.h client.h
	$(CC) main.c `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

play_song.o: play_song.c play_song.h
	$(CC) play_song.c `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

server.o: server.c server.h networking.h play_song.h
	$(CC) server.c `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

client.o: client.c client.h networking.h play_song.h
	$(CC) client.c `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

queue.o: queue.c queue.h library.h
	$(CC) queue.c `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

networking.o: networking.c networking.h
	$(CC) networking.c

library.o: library.c library.h
	$(CC) library.c

run:
	./program

clean:
	rm program
	rm *.o
	rm *~
