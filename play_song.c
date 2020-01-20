# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <gtk/gtk.h>

GtkWidget * enter_song;

void execute(char * program, char * argument) {
  int f = fork();
  if (!f) execlp(program, program, argument, NULL);
  waitpid(f, NULL, 0);
}

void clear() {
  execute("clear", NULL);
}

static int play_songs() {
    char full_path[100];
    strcpy(full_path, "songs/");
    full_path[strlen(full_path)] = 0;
    strcat(full_path, (char *)gtk_entry_get_text(GTK_ENTRY(enter_song)));
    strcat(full_path, ".wav");
    execute("aplay", full_path);
}

void play_song () {
    GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget * grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_set_row_spacing (GTK_GRID(grid), 25);

    GtkWidget * option = gtk_label_new("Play a song");
    gtk_grid_attach(GTK_GRID(grid), option, 0, 0, 1, 1);

    enter_song = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), enter_song, 0, 1, 1, 1);

    GtkWidget * button = gtk_button_new_with_label("Enter");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(play_songs), NULL);
    //
    // GtkWidget * list_all = gtk_button_new_with_label("List a");
    // gtk_grid_attach(GTK_GRID(grid), list_all, 0, 2, 1, 1);
    // g_signal_connect(list_all, "clicked", G_CALLBACK(list_all), NULL);

    // GtkWidget * button1 = gtk_button_new_with_label("Get a list of songs");
    // gtk_grid_attach(GTK_GRID(grid), button1, 0, 2, 1, 1);
    // g_signal_connect(button1, "clicked", G_CALLBACK(press_button1), NULL);
    //
    // GtkWidget * button2 = gtk_button_new_with_label("Connect to a game");
    // gtk_grid_attach(GTK_GRID(grid), button2, 0, 3, 1, 1);
    // g_signal_connect(button2, "clicked", G_CALLBACK(press_button2), NULL);

    gtk_widget_show_all(window);
    gtk_main();
  // char buffer[100];
  //
  // clear();
  // printf("Hello, you have entered the playing song interface\n\n");
  //
  // while (1) {
  //   printf("Type the name of a song to play it\n"
  //          "Type 'list all' to see a listing of all of our songs\n"
  //          "Type 'return' to return to the main selection screen\n\n");
  //   fgets(buffer, 100, stdin);
  //   buffer[strlen(buffer) - 1] = 0;
  //
  //   clear();

    // if (strcmp(buffer, "return") == 0) {
    //   execute("killall", "aplay");
    //   return;
    // }
    //
    // else if (strcmp(buffer, "terminate") == 0) {
    //   execute("killall", "aplay");
    // }
    //
    // else if (strcmp(buffer, "list all") == 0) {
    //   printf("This is what we've got:\n\n");
    //   execute("ls", "songs");
    //   printf("\n\n");
    // }
    //
    // else {
    //   printf("Attempting to play... %s\n\n", buffer);
    //   printf("Type 'terminate' to end currently playing song\n");
    //   if (!fork()) {
    //     char song_path[100] = "songs/";
    //     strcat(song_path, buffer);
    //     execlp("aplay", "aplay", song_path, NULL);
    //   }
    // }
}
