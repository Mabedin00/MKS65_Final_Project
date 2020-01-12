ifeq ($(DEBUG), true)
  CC = gcc -g -c
else
  CC = gcc -c
endif

all: main.o play_song.o server.o networking.o client.o
	gcc -o program main.o play_song.o server.o networking.o client.o

main.o: main.c play_song.h server.h client.h
	$(CC) main.c

play_song.o: play_song.c play_song.h
	$(CC) play_song.c

server.o: server.c server.h networking.h play_song.h
	$(CC) server.c

client.o: client.c client.h networking.h play_song.h
	$(CC) client.c

networking.o: networking.c networking.h
	$(CC) networking.c

run:
	./program

clean:
	rm program
	rm *.o
	rm *~
