# include <stdio.h>
# include <gtk/gtk.h>
# include <time.h>
# include <stdlib.h>
# include "networking.h"
# include "play_song.h"

GtkWidget * window;
GtkWidget * timer;
GtkWidget * alert;
GtkWidget * grid;
GtkWidget * points;
GtkWidget * ip_input;
GtkWidget * ip_label;
GtkWidget * username;
GtkWidget * username_label;
GtkWidget * option0;
GtkWidget * option1;
GtkWidget * option2;
GtkWidget * option3;
GtkWidget * ip_input_label;
GtkWidget * username_input_label;

char options[30][BUFFER_SIZE];

int arr[4];
int counter = 15;
int points_int = 0;
int num_songs; // counts down from total_songs to 0
int total_songs;
int server_socket;
int game_start_client = 0;
int guessed_current_song = 0;

void scramble_order () {
    int random_integer;
    int temp;
    int i;
    for(i = 0; i < 4; i++) {
        random_integer = rand() % 4;
        temp = arr[random_integer];
        arr[random_integer] = arr[i];
        arr[i] = temp;
    }
}

// remove songs/ and .wav,
char * extract_song(char * raw_song) {
    raw_song += 6;
    raw_song[strlen(raw_song) - 4] = 0;
    printf("Extract: %s\n", raw_song);
    return raw_song;
}


void process_option(int option) {
    char send_buffer[BUFFER_SIZE];
    char read_buffer[BUFFER_SIZE];
    int current_song = total_songs - num_songs;

    strcpy(send_buffer, options[(current_song) * 4 + option]);
    write(server_socket, send_buffer, BUFFER_SIZE);
    read(server_socket, read_buffer, BUFFER_SIZE);
    printf("Read buffer: %s\n", read_buffer);
    if (!strcmp(read_buffer, "W") && !guessed_current_song) {
        points_int += counter;
        guessed_current_song = 1;
    }
    else if (!guessed_current_song) {
        guessed_current_song = 1;
        points_int--;
    }
}

static int option_pressed0() {process_option(0);}
static int option_pressed1() {process_option(1);}
static int option_pressed2() {process_option(2);}
static int option_pressed3() {process_option(3);}

static int return_to_main_page() {
    gtk_main_quit();
    gtk_main_quit();
    execlp("make", "make", "run", NULL);
}

static int update() {
    if (game_start_client) {
        int current_song = total_songs - num_songs;
        gtk_button_set_label(GTK_BUTTON(option0), options[(current_song * 4) + 0]);
        gtk_button_set_label(GTK_BUTTON(option1), options[(current_song * 4) + 1]);
        gtk_button_set_label(GTK_BUTTON(option2), options[(current_song * 4) + 2]);
        gtk_button_set_label(GTK_BUTTON(option3), options[(current_song * 4) + 3]);

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
            counter = 15;
            gtk_label_set_text(GTK_LABEL(alert), "New Song Playing!");
        }
        if (counter == 8) {
            gtk_label_set_text(GTK_LABEL(alert), "Try your luck!");
        }

        if (num_songs <= 0) {
            gtk_widget_destroy(timer);
            gtk_widget_destroy(alert);
            gtk_widget_destroy(option0);
            gtk_widget_destroy(option1);
            gtk_widget_destroy(option2);
            gtk_widget_destroy(option3);


            GtkWidget * button3 = gtk_button_new_with_label("Return to main page!");
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
    char * ptr = receive_buffer;
    printf("Received from server: [%s]\n", receive_buffer);

    char temp_options[30][BUFFER_SIZE];
    if (strcmp(receive_buffer, "N") != 0) {
        int i;
        // printf("%s\n", receive_buffer);
    	for (i = 0; ptr != NULL; i++) {
    		strcpy(temp_options[i], extract_song(strsep(&ptr, ",")));
            // printf("Song: [%d]: %s\n", i, options[i]);
    	}

        num_songs = i / 4;

        arr[0] = 0;
        arr[1] = 1;
        arr[2] = 2;
        arr[3] = 3;
        for (i = 0; i < num_songs; i++) {

            scramble_order();

            strcpy(options[(i * 4) + 0], temp_options[(i * 4) + arr[0]]);
            strcpy(options[(i * 4) + 1], temp_options[(i * 4) + arr[1]]);
            strcpy(options[(i * 4) + 2], temp_options[(i * 4) + arr[2]]);
            strcpy(options[(i * 4) + 3], temp_options[(i * 4) + arr[3]]);

        }
        total_songs = num_songs;
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
    gtk_widget_destroy(ip_input_label);
    gtk_widget_destroy(username_input_label);


    timer = gtk_label_new("Timer");
    gtk_grid_attach(GTK_GRID(grid), timer, 0, 0, 1, 1);
    gtk_label_set_text(GTK_LABEL(timer), "Waiting for more players");

    username_label = gtk_label_new(username_storage);
    gtk_grid_attach(GTK_GRID(grid), username_label, 0, 1, 1, 1);

    g_timeout_add_seconds (1, load_screen, NULL);
    g_timeout_add_seconds (1, update, NULL);

    alert = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), alert, 0, 4, 1, 1);

    points = gtk_label_new("Points");
    gtk_grid_attach(GTK_GRID(grid), points, 0, 5, 1, 1);

    // printf("options[0]: %s\n", options[0]);
    option0 = gtk_button_new_with_label(options[0]);
    gtk_grid_attach(GTK_GRID(grid), option0, 1, 0, 1, 1);
    g_signal_connect(option0, "clicked", G_CALLBACK(option_pressed0), NULL);

    option1 = gtk_button_new_with_label(options[1]);
    gtk_grid_attach(GTK_GRID(grid), option1, 1, 1, 1, 1);
    g_signal_connect(option1, "clicked", G_CALLBACK(option_pressed1), NULL);

    option2 = gtk_button_new_with_label(options[2]);
    gtk_grid_attach(GTK_GRID(grid), option2, 1, 2, 1, 1);
    g_signal_connect(option2, "clicked", G_CALLBACK(option_pressed2), NULL);

    option3 = gtk_button_new_with_label(options[3]);
    gtk_grid_attach(GTK_GRID(grid), option3, 1, 3, 1, 1);
    g_signal_connect(option3, "clicked", G_CALLBACK(option_pressed3), NULL);

    gtk_widget_show_all(window);
    gtk_main();
}


int client() {
  srand(time(NULL));

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_grid_set_row_spacing (GTK_GRID(grid), 25);

  ip_label = gtk_button_new_with_label("Submit");
  gtk_grid_attach(GTK_GRID(grid), ip_label, 0, 2, 2, 1);
  g_signal_connect(ip_label, "clicked", G_CALLBACK(log_ip), NULL);

  ip_input = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), ip_input, 1, 0, 1, 1);

  ip_input_label = gtk_label_new("Enter the ip address:");
  gtk_grid_attach(GTK_GRID(grid), ip_input_label, 0, 0, 1, 1);

  username = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), username, 1, 1, 1, 1);

  username_input_label = gtk_label_new("Enter a username:");
  gtk_grid_attach(GTK_GRID(grid), username_input_label, 0, 1, 1, 1);


  gtk_widget_show_all(window);
  gtk_main();
}
