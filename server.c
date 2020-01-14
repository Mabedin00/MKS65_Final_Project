#include "networking.h"
#include "server.h"
#include "play_song.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

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

int random_int(int min, int max){
    srand(time(0));
    return min + rand() % (max+1 - min);
    
}


char * random_song() {
	
    char dir_to_scan[256] = "songs";
	char *  song_name = malloc(100);
	song_name = "/songs"; 
	DIR * dir = opendir(dir_to_scan);
	struct dirent * direntry = readdir(dir);
    int count_files = -2;
    for ( ; direntry != NULL; direntry = readdir(dir)){
        count_files++;
        }
    if (count_files < 0) {
        printf("No songs in the directory\n");
    }
    rewinddir(dir);
    
    int random_file = random_int(3,5);
    count_files = 0;
    for (; count_files != random_file; direntry = readdir(dir), count_files++){
     }
    strcat(song_name, direntry->d_name);
	//printf("%s \n", song_name);
    //return "song/americanpie.wav";
	return song_name;
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
		//printf("%s qwewqeqwe \n", random_song());
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
