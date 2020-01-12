#include "networking.h"
#include "server.h"
#include "play_song.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char * random_song() {
  return "songs/americanpie.wav";
}


void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  while (read(client_socket, buffer, sizeof(buffer))) {

    // code that I added
    if (strcmp(buffer, "americanpie.wav") == 0) {
        break;
    }
    strcpy(buffer, "WRONG, DOOFUS");
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop

  printf("Sending signal from subserver to main server to stop blocking at read\n");
  strcpy(buffer, "You won!");

  write(client_socket, buffer, sizeof(buffer));
  close(client_socket);
  exit(0);
}




int server() {
  char buffer[100];
  int number_connections = 2;
  int f;
  int listen_socket;
  listen_socket = server_setup();

  clear();

  while (1) {

    // child
    printf("Waiting for connections...\n");
    int mypipe[2];
    int counter = 0;
    while (counter < number_connections) {
        int client_socket = server_connect(listen_socket);

	pipe(mypipe);

	f = fork();
        if (f == 0) {
	    dup2(mypipe[1], STDOUT_FILENO);
            subserver(client_socket);
	}
        else {
            counter++;
            close(client_socket);
	}

    }
    // super_duper = dup(STDIN_FILENO);
    dup2(mypipe[0], STDIN_FILENO);
    f = fork();
    if (!f) {
        execlp("aplay", "aplay", random_song(), NULL);
    }
    else {
	char input[100];
        read(STDIN_FILENO, input, sizeof(input));
	printf("Input: [%s]\n", input);
	printf("Game Over Nerds\n");
	execute("killall", "aplay");
        exit(0);
      }
  }
}

