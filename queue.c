#include <gtk/gtk.h>
#include <sys/wait.h>
#include "library.h"
#include "queue.h"

#define SMALL_BUFF_SIZE 100

GtkWidget * window;
GtkWidget * enter_song;
GtkWidget * list_of_songs;
struct songQ yourQ;

char song_queue[30][SMALL_BUFF_SIZE];

static int return_to_main_page() {
    gtk_main_quit();
    execlp("make", "make", "run", NULL);
}

static int add_song() {
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
    }

    char new_text[SMALL_BUFF_SIZE];
    strcpy(new_text, gtk_label_get_text(GTK_LABEL(list_of_songs)));
    enqueue(&yourQ, filename);
    filename = strstr(filename, "songs") + 6;
    filename[strlen(filename) - 4] = 0;
    strcat(new_text, filename);
    strcat(new_text, "\n");
    gtk_label_set_text(GTK_LABEL(list_of_songs), new_text);
    gtk_widget_destroy (dialog);

}

static int play_queue() {
    char buffer[100];
    while (yourQ.first != NULL) {
        dequeue(&yourQ, buffer);
        int f = fork();
        if (!f) {
            execlp("aplay", "aplay", buffer, NULL);
        }
        else {
            int w;
            wait(&w);
        }
    }
}

void queue() {
    yourQ.first = NULL;
    yourQ.last = NULL;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget * grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_set_row_spacing (GTK_GRID(grid), 25);

    GtkWidget * add_song_button = gtk_button_new_with_label("Add song");
    gtk_grid_attach(GTK_GRID(grid), add_song_button, 0, 0, 1, 1);
    g_signal_connect(add_song_button, "clicked", G_CALLBACK(add_song), NULL);

    list_of_songs = gtk_label_new("Your current queue:\n");
    gtk_grid_attach(GTK_GRID(grid), list_of_songs, 1, 0, 1, 3);

    GtkWidget * play_queue_button = gtk_button_new_with_label("Play your queue");
    gtk_grid_attach(GTK_GRID(grid), play_queue_button, 0, 1, 1, 1);
    g_signal_connect(play_queue_button, "clicked", G_CALLBACK(play_queue), NULL);

    GtkWidget * return_to_main_page_button = gtk_button_new_with_label("Return to main page!");
    gtk_grid_attach(GTK_GRID(grid), return_to_main_page_button, 0, 2, 1, 1);
    g_signal_connect(return_to_main_page_button, "clicked", G_CALLBACK(return_to_main_page), NULL);

    gtk_widget_show_all(window);
    gtk_main();
}
