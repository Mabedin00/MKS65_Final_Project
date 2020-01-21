# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <gtk/gtk.h>
# include "play_song.h"

GtkWidget * enter_song;
GtkWidget * window;

static int return_to_main_page() {
    gtk_main_quit();
    execlp("make", "make", "run", NULL);
}

void execute(char * program, char * argument) {
  int f = fork();
  if (!f) execlp(program, program, argument, NULL);
  waitpid(f, NULL, 0);
}

void clear() {
  execute("clear", NULL);
}

static int play_songs() {
    GtkWidget * dialog;
    dialog = gtk_file_chooser_dialog_new (NULL, GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, ("_Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("_Add"),
                                      GTK_RESPONSE_ACCEPT,NULL);

    GtkFileChooser * chooser = GTK_FILE_CHOOSER (dialog);


    int res = gtk_dialog_run (GTK_DIALOG (dialog));
    char * filename;
    if (res == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        if (fork()) execlp("aplay", "aplay", filename, NULL);
      }

    gtk_widget_destroy (dialog);
}

void play_song () {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget * grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_set_row_spacing (GTK_GRID(grid), 25);

    GtkWidget * option = gtk_label_new("Play a song");
    gtk_grid_attach(GTK_GRID(grid), option, 0, 0, 1, 1);


    GtkWidget * button = gtk_button_new_with_label("Click to select a song");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(play_songs), NULL);

    GtkWidget * return_to_main_page_button = gtk_button_new_with_label("Return to main page!");
    gtk_grid_attach(GTK_GRID(grid), return_to_main_page_button, 0, 5, 1, 1);
    g_signal_connect(return_to_main_page_button, "clicked", G_CALLBACK(return_to_main_page), NULL);

    gtk_widget_show_all(window);
    gtk_main();
}
