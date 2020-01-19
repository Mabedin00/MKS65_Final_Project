# include <stdio.h>
# include <gtk/gtk.h>
# include "networking.h"
# include "play_song.h"

GtkWidget * button;
GtkWidget * timer;
GtkWidget * grid;
int counter = 3;

void enter_guess(char * buffer) {
    strcpy(buffer, (char *)gtk_entry_get_text(GTK_ENTRY(button)));
}

void do_stuff() {
    printf("Ran out\n");
}

static int update_timer() {
    char str[100];
    sprintf(str, "%d", counter);
    counter--;
    gtk_label_set_text(GTK_LABEL(timer), str);
    if (counter < 0) do_stuff();
    return 1;
}

int client() {
  printf("In client\n");
  int server_socket;
  char buffer[BUFFER_SIZE];
  char ip[BUFFER_SIZE];
  //fgets(ip, BUFFER_SIZE, stdin);
  strcpy(ip, "127.0.0.1"); // for testing, change later
  server_socket = client_setup(ip);

  // clear();

  printf("%s\n", ip);
  printf("Waiting for more players\n");

  read(server_socket, buffer, BUFFER_SIZE);
  // printf("Received from subserver: %s\n", buffer);
  printf("Game starting\n");

  while (1) {

      gtk_init(NULL, NULL);
      GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

      grid = gtk_grid_new();
      gtk_container_add(GTK_CONTAINER(window), grid);
      gtk_grid_set_row_spacing (GTK_GRID(grid), 50);

      button = gtk_entry_new();
      gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

      timer = gtk_label_new("Timer");
      gtk_grid_attach(GTK_GRID(grid), timer, 0, 1, 1, 1);

      g_timeout_add_seconds (1, update_timer, NULL);

      GtkWidget * button2 = gtk_button_new_with_label("Guess!");
      gtk_grid_attach(GTK_GRID(grid), button2, 0, 2, 1, 1);

      g_signal_connect(button2, "clicked", G_CALLBACK(enter_guess), buffer);

      gtk_widget_show_all(window);
      gtk_main();

      write(server_socket, buffer, sizeof(buffer));
      *buffer = '\0';
      read(server_socket, buffer, sizeof(buffer));

      if (!strcmp(buffer, "die")) break;
  }
  printf("The game has ended. Thank you for playing\n");
}
