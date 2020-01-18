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

// Known bugs:

// 1. Need to sleep after connections recieved or subserver writes empty message to client (???)

// ADDED:
// 1. Fix weird text stuff
// 2. Multiple songs can play
// 3. Make it so songs cannot be repeated
// 4. Variable max players, max songs

// 2. Add timer
// 4. Points!
// 5. Winning message

int game_start = 0;
int game_over = 0;
int client_socket = 0;

int server() {
  // SETTINGS

  char num_songs[BUFFER_SIZE];
  int max_song_number = max_songs();
  printf("How many songs would you like to play total?\n");
  printf("You may choose at most %d songs"
         "(that is how many songs are in the songs directory)\n", max_song_number);

  //fgets(num_songs, BUFFER_SIZE, stdin);
  strcpy(num_songs, "3");

  while (atoi(num_songs) > max_song_number) {
    printf("Please select less songs than %d/n", max_song_number);
    fgets(num_songs, BUFFER_SIZE, stdin);
  }

  char max_players[BUFFER_SIZE];
  printf("What is the max number of players you would like to have?\n");
  printf("We recommend you choose at most 4 players. But choose as many as you want!\n");

  //fgets(max_players, BUFFER_SIZE, stdin);
  strcpy(max_players, "2");

  // ACTUAL GAME START

  char buffer[100];
  int number_connections = atoi(max_players);
  int f;
  int listen_socket;
  int current_song_number = 0;
  int num_songs_int = atoi(num_songs);
  listen_socket = server_setup();
  char * songs_to_be_played[num_songs_int];
  int pids[number_connections];
  srand(time(NULL));


  clear();
  printf("Waiting for connections...\n");

  int counter = 0;
  while (counter < num_songs_int) {
    songs_to_be_played[counter] = random_song();

    while (is_duplicate(songs_to_be_played, counter))
      songs_to_be_played[counter] = random_song();
    counter++;
  }

  counter = 0;
  while (counter < number_connections) {
    client_socket = server_connect(listen_socket);

      pids[counter] = fork();
      if (pids[counter] == 0) {
        signal(SIGHUP, sighandler_2);
        signal(SIGALRM, sighandler_3);
        subserver(client_socket, songs_to_be_played);
      }
      else {
        counter++;
        close(client_socket);
    }
  }

  sleep(1); // why does this have to be here? to be honest, I don't know

  int i;
  for (i = 0; i < number_connections; i++) {
    kill(pids[i], SIGALRM); // tell subservers game is starting
  }

  while (current_song_number < num_songs_int) {

    f = fork();
    if (!f) {
        execlp("aplay", "aplay", songs_to_be_played[current_song_number], NULL);
    }

    // server that's not playing the song
    else {

      signal(SIGSYS, sighandler);
      while(1) {
        if (game_over == 1) break;
	sleep(1);
      }

      game_over = 0; // reset game_over
      sleep(1);

      int i;
      for (i = 0; i < number_connections; i++) {
        kill(pids[i], SIGHUP); // tell subservers to move on to next song
      }

    } // end server else

    current_song_number++;
  } // end of while current_song < num_songs loop
} // end of server function


void subserver(int client_socket, char ** songs_to_be_played) {
  char receive_buffer[BUFFER_SIZE];
  char send_buffer[BUFFER_SIZE];

  int current_song = 0;

  while (!game_start) sleep(.1);

  strcpy(send_buffer, "Game start\n");
  write(client_socket, send_buffer, BUFFER_SIZE);

  while (read(client_socket, receive_buffer, BUFFER_SIZE)) {

    if (game_over == 1) { // if we should move onto next song
      game_over = 0;
      current_song++;
    }

    if (!strcmp(receive_buffer, songs_to_be_played[current_song])) {
      kill(getppid(), SIGSYS);
      strcpy(send_buffer, "You won!");
      write(client_socket, send_buffer, sizeof(send_buffer));
    }
    else {
      strcpy(send_buffer, "Incorrect guess");
      write(client_socket, send_buffer, sizeof(send_buffer));
    }
  }//end read loop

  close(client_socket);
  exit(0);
}



int random_int(int min, int max) {
    return min + rand() % (max+1 - min);
}

void sighandler() {
  printf("Game over nerds\n");
  execute("killall", "aplay");
  game_over = 1;
}

void sighandler_2() {
  //char buffer[BUFFER_SIZE];
  //printf("Ran sighandler2\n");
  //strcpy(buffer, "Sorry but game's over\n");
  //write(client_socket, buffer, sizeof(buffer));
  game_over = 1; // tell subservers we're moving onto next song
}

void sighandler_3() {
  game_start = 1; // tell subservers game has started
}

char * random_song() {
    int default_dir_count = 2;
    char dir_to_scan[BUFFER_SIZE] = "songs";
    char *  song_name = malloc(100);
    strcpy(song_name, "songs/");
    DIR * dir = opendir(dir_to_scan);
    struct dirent * direntry = readdir(dir);
    int count_files = -default_dir_count;
    for ( ; direntry != NULL; direntry = readdir(dir)){
        count_files++;
    }

    if (count_files < 0) {
        printf("No songs in the directory\n");
    }
    rewinddir(dir);
    int random_file = random_int(default_dir_count + 1, default_dir_count + count_files);
    count_files = 0;

    for (; count_files != random_file; direntry = readdir(dir), count_files++){
     }
    strcat(song_name, direntry->d_name);
    return song_name;
}

int is_duplicate(char ** songs_to_be_played, int index) {
    int i;
    for (i = 0; i < index; i++)
      if (!strcmp(songs_to_be_played[i], songs_to_be_played[index]))
        return 1;
    return 0;
}

int max_songs() {
    char dir_to_scan[BUFFER_SIZE] = "songs";
    DIR * dir = opendir(dir_to_scan);
    struct dirent * direntry = readdir(dir);
    int default_dir_count = 2;
    int count_files = -default_dir_count;
    for ( ; direntry != NULL; direntry = readdir(dir)){
        count_files++;
    }
    return count_files;
}
