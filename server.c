#include "networking.h"
#include "server.h"
#include "play_song.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

int game_over = 0;
int client_socket = 0;

void sighandler() {
  printf("Game over nerds\n");
  execute("killall", "aplay");
  game_over = 1;
}

void sighandler_2() {
  char buffer[BUFFER_SIZE];
  printf("Ran sighandler2\n");
  strcpy(buffer, "Sorry but game's over\n");
  write(client_socket, buffer, sizeof(buffer));
}

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

  kill(getppid(), SIGSYS);
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
  int pids[number_connections];

  while (1) {

    // child
    printf("Waiting for connections...\n");
    //int mypipe[2];
    int counter = 0;

    while (counter < number_connections) {
        client_socket = server_connect(listen_socket);

	//pipe(mypipe);

	pids[counter] = fork();
        if (pids[counter] == 0) {
	    signal(SIGHUP, sighandler_2);
	    //dup2(mypipe[1], STDOUT_FILENO);
            subserver(client_socket);
	}
        else {
            counter++;
            close(client_socket);
	}

    }
    // super_duper = dup(STDIN_FILENO);
    // dup2(mypipe[0], STDIN_FILENO);
    f = fork();
    if (!f) {
        execlp("aplay", "aplay", random_song(), NULL);
    }
    else {
        signal(SIGSYS, sighandler);
	while(1) {
	  if (game_over == 1) break;
	  sleep(1);
      }
	printf("We got out of the loop\n");
	
	int i;
	for (i = 0; i < number_connections; i++) {
	  kill(pids[i], SIGHUP); // just used for communications
      }
    }
  }
}

