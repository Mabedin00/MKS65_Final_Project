# include <stdio.h>
# include <gtk/gtk.h>
# include "networking.h"
# include "play_song.h"

GtkWidget * window;
GtkWidget * button;
GtkWidget * button2;
GtkWidget * timer;
GtkWidget * alert;
GtkWidget * grid;
GtkWidget * points;
GtkWidget * ip_input;
GtkWidget * ip_label;
GtkWidget * username;
GtkWidget * username_label;

int counter = 5;
int points_int = 0;
int num_songs;
int server_socket;
int game_start_client = 0;
int guessed_current_song = 0;

void enter_guess() {
    char send_buffer[BUFFER_SIZE];
    char read_buffer[BUFFER_SIZE];

    strcpy(send_buffer, (char *)gtk_entry_get_text(GTK_ENTRY(button)));
    write(server_socket, send_buffer, BUFFER_SIZE);
    read(server_socket, read_buffer, BUFFER_SIZE);
    if (!strcmp(read_buffer, "W") && !guessed_current_song) {
        points_int += counter;
        guessed_current_song = 1;
    }
    else if (!guessed_current_song) {
        points_int--;
    }
}

static int return_to_main_page() {
    gtk_main_quit();
    gtk_main_quit();
    execlp("make", "make", "run", NULL);
}

static int update() {
    if (game_start_client) {

        counter--;
        char str[BUFFER_SIZE];
        sprintf(str, "%d", counter);
        char message[BUFFER_SIZE] = "Time remaining: ";
        strcat(message, str);
        gtk_label_set_text(GTK_LABEL(timer), message);

        char points_display[BUFFER_SIZE];
        sprintf(points_display, "%d", points_int);
        gtk_label_set_text(GTK_LABEL(points), points_display);

        if (counter <= 0) {
            guessed_current_song = 0;
            num_songs--;
            counter = 5;
            gtk_label_set_text(GTK_LABEL(alert), "New Song Playing!");
        }
        if (counter == 8) {
            gtk_label_set_text(GTK_LABEL(alert), "Try your luck!");
        }

        if (num_songs <= 0) {
            gtk_widget_destroy(button);
            gtk_widget_destroy(button2);
            gtk_widget_destroy(timer);
            gtk_widget_destroy(alert);

            GtkWidget * button3 = gtk_button_new_with_label("Return to MAIN page!");
            gtk_grid_attach(GTK_GRID(grid), button3, 0, 2, 1, 1);
            g_signal_connect(button3, "clicked", G_CALLBACK(return_to_main_page), NULL);

            gtk_widget_show_all(window);
            gtk_main();
        }
        return 1;
    }
    else {
        return 1;
    }
}

static int load_screen()  {
    char receive_buffer[BUFFER_SIZE];
    read(server_socket, receive_buffer, BUFFER_SIZE);
    // printf("Received from server: [%s]\n", receive_buffer);
    if (strcmp(receive_buffer, "N") != 0) {
        num_songs = atoi(receive_buffer);
        game_start_client = 1;
        return 0;
    }
    return 1;
}

static int log_ip() {
    char ip[BUFFER_SIZE];
    char username_storage[BUFFER_SIZE];
    strcpy(username_storage,(char *)gtk_entry_get_text(GTK_ENTRY(username)));

    strcpy(ip, (char *)gtk_entry_get_text(GTK_ENTRY(ip_input)));
    server_socket = client_setup(ip);

    gtk_widget_destroy(ip_label);
    gtk_widget_destroy(ip_input);
    gtk_widget_destroy(username);

    timer = gtk_label_new("Timer");
    gtk_grid_attach(GTK_GRID(grid), timer, 0, 0, 1, 1);
    gtk_label_set_text(GTK_LABEL(timer), "Waiting for more players");

    username_label = gtk_label_new(username_storage);
    gtk_grid_attach(GTK_GRID(grid), username_label, 0, 1, 1, 1);

    g_timeout_add_seconds (1, update, NULL);
    g_timeout_add_seconds (1, load_screen, NULL);

    button2 = gtk_button_new_with_label("Guess!");
    gtk_grid_attach(GTK_GRID(grid), button2, 0, 2, 1, 1);
    g_signal_connect(button2, "clicked", G_CALLBACK(enter_guess), NULL);

    button = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 1, 1);

    alert = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), alert, 0, 4, 1, 1);

    points = gtk_label_new("Points");
    gtk_grid_attach(GTK_GRID(grid), points, 0, 5, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();
}


int client() {
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_grid_set_row_spacing (GTK_GRID(grid), 25);

  ip_label = gtk_button_new_with_label("Enter (1. Desired IP) (2. Your username):");
  gtk_grid_attach(GTK_GRID(grid), ip_label, 0, 0, 1, 1);
  g_signal_connect(ip_label, "clicked", G_CALLBACK(log_ip), NULL);

  ip_input = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), ip_input, 0, 1, 1, 1);

  username = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), username, 0, 2, 1, 1);

  gtk_widget_show_all(window);
  gtk_main();
}
