#include "networking.h"
#include "server.h"
#include "play_song.h"
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>


// TODO:

// 0. Repeat songs
// 1. Make it so songs cannot be repeated
// 2. Add timer
// 3. Variable max players, max songs
// 4. Points!
// 5. Winning message

int game_over = 0;
int client_socket = 0;

int server(char * num_songs) {
  char buffer[100];
  int number_connections = 2;
  int f;
  int listen_socket;
  int current_song_number = 0;
  int num_songs_int = atoi(num_songs);
  listen_socket = server_setup();
  char * song_to_be_played;
  int pids[number_connections];

  clear();
  printf("Waiting for connections...\n");

  song_to_be_played = random_song();

  int counter = 0;
  while (counter < number_connections) {
    client_socket = server_connect(listen_socket);

      pids[counter] = fork();
      if (pids[counter] == 0) {
        signal(SIGHUP, sighandler_2);
        subserver(client_socket, song_to_be_played);
      }
      else {
        counter++;
        close(client_socket);
    }
  }

  while (current_song_number < num_songs_int) {
    // for the fist song, song_to_be_played must be determined outside of while loop
    if (current_song_number != 0) {
      *song_to_be_played = '\0'; // wipe contents of song_to_be_played
      song_to_be_played = random_song();
    }

    f = fork();
    if (!f) {
        execlp("aplay", "aplay", song_to_be_played, NULL);
    }

    // server that's not playing the song
    else {

      signal(SIGSYS, sighandler);
      while(1) {
        if (game_over == 1) break;
	sleep(1);
      }

      game_over = 0; // reset game_over
      sleep(3);

      int i;
      for (i = 0; i < number_connections; i++) {
        kill(pids[i], SIGHUP); // SIGHUP just used for communications
      }

    } // end server else

    current_song_number++;
  } // end of while current_song < num_songs loop
} // end of server function


void subserver(int client_socket, char * song_to_be_played) {
  char receive_buffer[BUFFER_SIZE];
  char send_buffer[BUFFER_SIZE];

  while (read(client_socket, receive_buffer, sizeof(receive_buffer))) {

    // code that I added
    if (!strcmp(receive_buffer, song_to_be_played)) {
      kill(getppid(), SIGSYS);
      strcpy(send_buffer, "You won!");
      write(client_socket, send_buffer, sizeof(send_buffer));
    }

    strcpy(send_buffer, "Incorrect guess");
    write(client_socket, send_buffer, sizeof(send_buffer));
  }//end read loop

  close(client_socket);
  exit(0);
}



int random_int(int min, int max){
    srand(time(0));
    return min + rand() % (max+1 - min);

}

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

    char dir_to_scan[256] = "songs";
	char *  song_name = malloc(100);
	strcpy(song_name, "songs/");
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
    // return "songs/americanpie.wav";
	return song_name;

}
