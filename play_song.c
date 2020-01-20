# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <gtk/gtk.h>
# include "play_song.h"

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

    gtk_widget_show_all(window);
    gtk_main();
}
