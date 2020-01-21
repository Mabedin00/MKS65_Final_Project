# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <gtk/gtk.h>
# include "play_song.h"
# include "server.h"
# include "client.h"
# include "queue.h"

GtkWidget * window;

void press_button() {
    gtk_widget_destroy(window);
    play_song();
}
void press_button1() {
    gtk_widget_destroy(window);
    server();
}

void press_button2() {
    gtk_widget_destroy(window);
    client();
}

void press_button3() {
    gtk_widget_destroy(window);
    queue();
}

int main() {
    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget * grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_set_row_spacing (GTK_GRID(grid), 25);
    gtk_grid_set_column_spacing (GTK_GRID(grid), 10);


    GtkWidget * option = gtk_label_new("Select an option");
    gtk_grid_attach(GTK_GRID(grid), option, 0, 0, 2, 1);

    GtkWidget * welcome = gtk_label_new("\n\n\n\n\nHello! Welcome to BEATZ(tm)\n\n\n\n"
                                       "You can play a song, add songs\n\n\n\n"
                                       "to a queue, or play our original\n\n\n\n"
                                       "ka-BEATZ(tm) game!\n");
    gtk_grid_attach(GTK_GRID(grid), welcome, 1, 0, 1, 5);

    GtkWidget * button = gtk_button_new_with_label("Play a song");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(press_button), NULL);

    GtkWidget * button1 = gtk_button_new_with_label("Host a game");
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 3, 1, 1);
    g_signal_connect(button1, "clicked", G_CALLBACK(press_button1), NULL);

    GtkWidget * button2 = gtk_button_new_with_label("Connect to a game");
    gtk_grid_attach(GTK_GRID(grid), button2, 0, 4, 1, 1);
    g_signal_connect(button2, "clicked", G_CALLBACK(press_button2), NULL);

    GtkWidget * button3 = gtk_button_new_with_label("Create a queue of songs");
    gtk_grid_attach(GTK_GRID(grid), button3, 0, 2, 1, 1);
    g_signal_connect(button3, "clicked", G_CALLBACK(press_button3), NULL);

    gtk_widget_show_all(window);
    gtk_main();
}
