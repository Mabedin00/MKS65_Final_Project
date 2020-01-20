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

// SIGSYS: subserver -> server (song guessed correctly)
// SIGHUP: server -> all subservers (song guessed correctly)
// SIGALRM: server -> subserver (game has started / all users connected)
// SIGILL: server -> subserver (game has ended / played last song)

int game_start = 0;
int game_over = 0;
int client_socket = 0;
int ran_run_server_code = 0;
int user_input_songs = 0;
int user_input_players = 0;

GtkWidget * submit;
GtkWidget * song;
GtkWidget * count_players;
GtkWidget * count_songs;
GtkWidget * explanation;
GtkWidget * window;

static int init_server() {
    user_input_songs = atoi((char *)gtk_entry_get_text(GTK_ENTRY(count_songs)));
    int max_song_number = max_songs();

    if (user_input_songs > max_song_number) {
        gtk_label_set_text(GTK_LABEL(explanation), "You entered too many songs");
        gtk_widget_show_all(window);
    }
    else {
        gtk_widget_destroy(count_songs);
        gtk_widget_destroy(submit);
        gtk_label_set_text(GTK_LABEL(explanation), "Server now running!");
        user_input_players = atoi((char *)gtk_entry_get_text(GTK_ENTRY(count_players)));
        gtk_widget_destroy(count_players);
        gtk_widget_show_all(window);
        g_timeout_add_seconds(1, run_server_code, NULL);
        gtk_main();
        return 0;
    }
    return 1;
}

static int return_to_main_page() {
    gtk_main_quit();
    gtk_main_quit();
    gtk_main_quit();
    gtk_main_quit();
    execlp("make", "make", "run", NULL);
}

static int run_server_code() {
    printf("Running run_server_code\n");
    if (!ran_run_server_code) {
        ran_run_server_code = 1;
        printf("Running run_server_code inside if\n");
        int listen_socket = server_setup();
        int current_song_number = 0;
        int pids[user_input_players];
        char * songs_to_be_played[user_input_songs];
        char buffer[0];

        srand(time(NULL)); // needed later for random_song()

        clear();
        printf("Waiting for connections...\n");
        // populate songs_to_be_played with random songs
        int counter = 0;
        printf("user_input_songs: [%d]\n", user_input_songs);
        while (counter < user_input_songs) {
          songs_to_be_played[counter] = random_song();
          while (is_duplicate(songs_to_be_played, counter)) {
            songs_to_be_played[counter] = random_song();
          }
          counter++;
        }

        // get all players connected
        counter = 0;
        printf("user_input_players: [%d]\n", user_input_players);
        while (counter < user_input_players) {
          client_socket = server_connect(listen_socket);
            pids[counter] = fork();
            if (pids[counter] == 0) {
              signal(SIGHUP, sighandler_2);
              signal(SIGALRM, sighandler_3);
              signal(SIGILL, sighandler_4);
              subserver(client_socket, songs_to_be_played);
            }
            else {
              counter++;
              close(client_socket);
          }
        }

        // tell subservers game is starting
        sleep(1);
        int i;
        for (i = 0; i < user_input_players; i++) {
          // printf("Sending SIGALRM to [%d]\n", pids[i]);
          kill(pids[i], SIGALRM);
        }

        while (current_song_number < user_input_songs) {
          int f = fork();
          // one process plays the song
          if (!f) execlp("aplay", "aplay", songs_to_be_played[current_song_number], NULL);
          // server that's not playing the song
          else {
            f = fork();
            // this one is just a clock
            if (!f) {
               sleep(6);
               kill(getppid(), SIGSYS);
               exit(0);
            }
            // waits for song to be guessed or <amount of time> to elapse
            else {

              signal(SIGSYS, sighandler);
              while(!game_over) sleep(.1);
              // reset game_over
              game_over = 0;
              // tell subservers to move on to next song
              int i;
              for (i = 0; i < user_input_players; i++) {
                kill(pids[i], SIGHUP);
              }
            }
          } // end server else
          current_song_number++;
        }
        for (i = 0; i < user_input_players; i++) {
          kill(pids[i], SIGILL);
        }
        return_to_main_page();
    }
    else {
        return 0;
    }
}

int server() {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget * grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_set_row_spacing (GTK_GRID(grid), 25);

    submit = gtk_button_new_with_label("Click to submit selections (1. players) (2. songs)");
    gtk_grid_attach(GTK_GRID(grid), submit, 0, 0, 1, 1);
    g_signal_connect(submit, "clicked", G_CALLBACK(init_server), NULL);

    count_players = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), count_players, 0, 1, 1, 1);

    count_songs = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), count_songs, 0, 2, 1, 1);

    char explanation_label[BUFFER_SIZE] = "Please input at most ";
    char num_songs[BUFFER_SIZE];
    sprintf(num_songs, "%d", max_songs());
    strcat(explanation_label, num_songs);
    strcat(explanation_label, " songs. (That is how many are in your songs directory)");

    explanation = gtk_label_new(explanation_label);
    gtk_grid_attach(GTK_GRID(grid), explanation, 0, 3, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();
}

void subserver(int client_socket, char ** songs_to_be_played) {
  char receive_buffer[BUFFER_SIZE];
  char full_path[BUFFER_SIZE];
  char send_buffer[BUFFER_SIZE];
  int current_song = 0;

  // wait until game start
  while (!game_start) {
      write(client_socket, send_buffer, BUFFER_SIZE);
      sleep(1);
  }

  char send_data[BUFFER_SIZE];
  // telling clients how many songs will be playing total
  sprintf(send_data, "%d", user_input_songs);
  write(client_socket, send_data, BUFFER_SIZE);

  while (current_song < user_input_songs && read(client_socket, receive_buffer, BUFFER_SIZE)) {
    // if we should move onto next song
    if (game_over > 0) {
      // in case game_over is > 1 (another user has guessed correctly twice)
      current_song += game_over;
      game_over = 0;
    }
    // if someone guesses correct song
    strcpy(full_path, "songs/");
    full_path[strlen(full_path)] = 0;
    strcat(full_path, receive_buffer);
    strcat(full_path, ".wav");

    if (!strcmp(full_path, songs_to_be_played[current_song])) {
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
}

void sighandler_4() {
  char send_buffer[BUFFER_SIZE];
  strcpy(send_buffer, "die");
  write(client_socket, send_buffer, BUFFER_SIZE);
  close(client_socket);
  exit(0);
}
