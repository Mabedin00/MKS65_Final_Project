# include <stdio.h>
# include <gtk/gtk.h>
# include "networking.h"
# include "play_song.h"

GtkWidget * button;
GtkWidget * timer;
GtkWidget * alert;
GtkWidget * grid;
int counter = 10;
int num_songs;
int server_socket;

void enter_guess() {
    char send_buffer[BUFFER_SIZE];
    char read_buffer[BUFFER_SIZE];

    printf("enter_guess triggered\n");
    strcpy(send_buffer, (char *)gtk_entry_get_text(GTK_ENTRY(button)));
    write(server_socket, send_buffer, BUFFER_SIZE);
    // read(server_socket, read_buffer, BUFFER_SIZE);

    // printf("Received: %s\n", read_buffer);
}

void do_stuff() {
    printf("do_stuff triggered\n");
}

static int update() {
    counter--;

    char str[100];
    sprintf(str, "%d", counter);
    char message[100] = "Time remaining: ";
    strcat(message, str);
    gtk_label_set_text(GTK_LABEL(timer), message);

    if (counter < 0) {
        num_songs--;
        counter = 10;
        gtk_label_set_text(GTK_LABEL(alert), "New Song Playing!");
    }
    if (counter == 8) {
        gtk_label_set_text(GTK_LABEL(alert), "");
    }

    if (num_songs <= 0) {
        gtk_main_quit();
    }
    return 1;
}

int client() {
  char receive_buffer[BUFFER_SIZE];
  char ip[BUFFER_SIZE];
  //fgets(ip, BUFFER_SIZE, stdin);
  strcpy(ip, "127.0.0.1"); // for testing, change later
  server_socket = client_setup(ip);

  // clear();

  // printf("%s\n", ip);
  // printf("Waiting for more players\n");
  //
  // read(server_socket, buffer, BUFFER_SIZE);
  // // printf("Received from subserver: %s\n", buffer);
  // printf("Game starting\n");

  while (1) {

      gtk_init(NULL, NULL);
      GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

      grid = gtk_grid_new();
      gtk_container_add(GTK_CONTAINER(window), grid);
      gtk_grid_set_row_spacing (GTK_GRID(grid), 25);

      button = gtk_entry_new();
      gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

      timer = gtk_label_new("Timer");
      gtk_grid_attach(GTK_GRID(grid), timer, 0, 1, 1, 1);
      gtk_label_set_text(GTK_LABEL(timer), "Waiting for more players");

      alert = gtk_label_new("");
      gtk_grid_attach(GTK_GRID(grid), alert, 0, 2, 1, 1);

      g_timeout_add_seconds (1, update, NULL);
      // g_timeout_add_seconds (1, get_data_from_server, NULL);

      GtkWidget * button2 = gtk_button_new_with_label("Guess!");
      gtk_grid_attach(GTK_GRID(grid), button2, 0, 3, 1, 1);

      g_signal_connect(button2, "clicked", G_CALLBACK(enter_guess), NULL);


      read(server_socket, receive_buffer, BUFFER_SIZE);
      num_songs = atoi(receive_buffer);

      gtk_widget_show_all(window);
      gtk_main();

      // printf("receive_buffer: %s\n", receive_buffer);
      // *buffer = '\0';
      //
      // if (!strcmp(buffer, "die")) break;
  }
  printf("The game has ended. Thank you for playing\n");
}
