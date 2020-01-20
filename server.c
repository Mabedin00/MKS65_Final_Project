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
#include <gtk/gtk.h>


//TODO:

// Visual play song
// -2. Get queue to work          DON'T NEED
// 0. Input IP
// 1. Winning message             CHECK
// 2. Don't move on               DON'T NEED
// 3. Server has their own popup
// 4. Ask for count players, songs
// 5. Sync counter                DON'T NEED
// 6. Add songs in some way       DON'T NEED
// 7. Points, all
// 8. Make interface pretty

// ** comment out aplay, test on skewl machines

// SIGSYS: subserver -> server (song guessed correctly)
// SIGHUP: server -> all subservers (song guessed correctly)
// SIGALRM: server -> subserver (game has started / all users connected)
// SIGILL: server -> subserver (game has ended / played last song)

int game_start = 0;
int game_over = 0;
int client_socket = 0;
int ran_run_server_code = 0;

GtkWidget * song;

static int return_to_main_page() {
    gtk_main_quit();
    gtk_main_quit();
    execlp("make", "make", "run", NULL);
}

static int run_server_code() {
    printf("Running run_server_code\n");
    if (!ran_run_server_code) {
        ran_run_server_code = 1;
        printf("Running run_server_code inside if\n");
        int num_songs = ask_for_num_songs();
        int number_connections = ask_for_num_players();
        int max_song_number = max_songs();
        int listen_socket = server_setup();
        int current_song_number = 0;
        int pids[number_connections];
        char * songs_to_be_played[num_songs];
        char buffer[100];

        srand(time(NULL)); // needed later for random_song()

        clear();
        printf("Waiting for connections...\n");
        // populate songs_to_be_played with random songs
        int counter = 0;
        while (counter < num_songs) {
          songs_to_be_played[counter] = random_song();
          while (is_duplicate(songs_to_be_played, counter)) {
            songs_to_be_played[counter] = random_song();
          }
          counter++;
        }

        // get all players connected
        counter = 0;
        while (counter < number_connections) {
          client_socket = server_connect(listen_socket);
            pids[counter] = fork();
            if (pids[counter] == 0) {
              signal(SIGHUP, sighandler_2);
              signal(SIGALRM, sighandler_3);
              signal(SIGILL, sighandler_4);
              subserver(client_socket, songs_to_be_played, max_song_number);
            }
            else {
              counter++;
              close(client_socket);
              // if (counter < number_connections - 1) exit(0);
          }
        }

        sleep(1);
        // tell subservers game is starting
        int i;
        for (i = 0; i < number_connections; i++) {
          // printf("Sending SIGALRM to [%d]\n", pids[i]);
          kill(pids[i], SIGALRM);
        }

        while (current_song_number < num_songs) {
          int f = fork();
          // one process plays the song
          if (!f) {
              gtk_label_set_text(GTK_LABEL(song), songs_to_be_played[current_song_number]);
              execlp("aplay", "aplay", songs_to_be_played[current_song_number], NULL);
          }
          // server that's not playing the song
          else {
            f = fork();
            // this one is just a clock
            if (!f) {
               sleep(3);
               kill(getppid(), SIGSYS);
               exit(0);
            }
            // waits for song to be guessed or 15s to elapse
            else {

              signal(SIGSYS, sighandler);
              while(!game_over) sleep(.1);
              // reset game_over
              game_over = 0;
              // tell subservers to move on to next song
              int i;
              for (i = 0; i < number_connections; i++) {
                kill(pids[i], SIGHUP);
              }
            }
          } // end server else
          current_song_number++;
        }
        for (i = 0; i < number_connections; i++) {
          kill(pids[i], SIGILL);
        }
        return_to_main_page();
    }
    else {
        return 1;
    }
}

int server() {
    GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget * grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_set_row_spacing (GTK_GRID(grid), 25);

    GtkWidget * label = gtk_label_new("This is the server");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    song = gtk_label_new("Current song: ");
    gtk_grid_attach(GTK_GRID(grid), song, 0, 1, 1, 1);

    g_timeout_add_seconds (1, run_server_code, NULL);

    gtk_widget_show_all(window);
    gtk_main();
}

void subserver(int client_socket, char ** songs_to_be_played, int max_song_number) {
  char receive_buffer[BUFFER_SIZE];
  char full_path[BUFFER_SIZE];
  char send_buffer[BUFFER_SIZE];
  int current_song = 0;

  // wait until game start

  strcpy(send_buffer, "N");
  // printf("Game status (subserver): [%d]\n", game_start);
  while (!game_start) {
      // printf("[%d] game status in loop: [%d]\n", getpid(), game_start);
      write(client_socket, send_buffer, BUFFER_SIZE);
      sleep(1);
  }

  char send_data[BUFFER_SIZE];
  sprintf(send_data, "%d", max_song_number);
  // printf("Wrote %s to client\n", send_data);
  write(client_socket, send_data, BUFFER_SIZE);

  while (current_song < max_song_number && read(client_socket, receive_buffer, BUFFER_SIZE)) {
    if (game_over > 0) { // if we should move onto next song
      current_song += game_over; // in case game_over is > 1 (another user has guessed correctly twice)
      game_over = 0;
    }
    // if someone guesses correct song
    strcpy(full_path, "songs/");

    full_path[strlen(full_path)] = 0;

    strcat(full_path, receive_buffer);
    strcat(full_path, ".wav");

    printf("Full path: %s\n", full_path);
    printf("Current song: %d\n", current_song);

    if (!strcmp(full_path, songs_to_be_played[current_song])) {
      // kill(getppid(), SIGSYS);
      strcpy(send_buffer, "W");
      write(client_socket, send_buffer, BUFFER_SIZE);
    }
    else {
      strcpy(send_buffer, "Incorrect guess");
      write(client_socket, send_buffer, BUFFER_SIZE);
    }
  }
}

int random_int(int min, int max) {
    return min + rand() % (max + 1 - min);
}

char * random_song() {
    char * song_name = malloc(BUFFER_SIZE);
    char dir_to_scan[BUFFER_SIZE] = "songs";
    strcpy(song_name, "songs/");

    DIR * dir = opendir(dir_to_scan);
    struct dirent * direntry = readdir(dir);

    int count_files;
    for (count_files = 0; direntry != NULL; direntry = readdir(dir)){
        count_files++;
    }

    rewinddir(dir);
    int random_file = random_int(1, count_files);
    // reusing count_files variable as a counter
    for (count_files = 0; count_files != random_file; direntry = readdir(dir), count_files++) {}

    strcat(song_name, direntry->d_name);
    return song_name;
}

// check if is already inside, or if is '.' or '..'
int is_duplicate(char ** songs_to_be_played, int index) {
    int i;
    for (i = 0; i < index; i++) {
      if (!strcmp(songs_to_be_played[i], songs_to_be_played[index])) {
        return 1;
      }
    }
    if (!strcmp(songs_to_be_played[index], "songs/.")
     || !strcmp(songs_to_be_played[index], "songs/.."))
        return 1;
    return 0;
}

int max_songs() {
    char dir_to_scan[BUFFER_SIZE] = "songs";
    DIR * dir = opendir(dir_to_scan);
    struct dirent * direntry = readdir(dir);

    int count_files;
    for (count_files = 0; direntry != NULL; direntry = readdir(dir)){
        count_files++;
    }
    return count_files - 2; // for '.' and '..', which we don't want to count
}

int ask_for_num_songs() {
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
    return atoi(num_songs);
}

int ask_for_num_players() {
    char max_players[BUFFER_SIZE];
    printf("What is the max number of players you would like to have?\n");
    printf("We recommend you choose at most 4 players. But choose as many as you want!\n");
    //fgets(max_players, BUFFER_SIZE, stdin);
    strcpy(max_players, "2");
    return atoi(max_players);
}

void sighandler() {
    printf("Game over nerds\n");
    execute("killall", "aplay");
    game_over = 1;
}

void sighandler_2() {
    game_over++; // tell subservers we're moving onto next song
}

void sighandler_3() {
    game_start = 1; // tell subservers game has started
    // printf("Game status (subservers): [%d]\n", game_start);
}

void sighandler_4() {
  char send_buffer[BUFFER_SIZE];
  strcpy(send_buffer, "die");
  write(client_socket, send_buffer, BUFFER_SIZE);
  close(client_socket);
  exit(0);
}
