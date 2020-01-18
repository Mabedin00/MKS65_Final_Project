#include "networking.h"

void sighandler();
void sighandler_2();
int random_int(int min, int max);
int server(char * num_songs);
char * random_song();
void subserver(int client_socket, char * song_to_be_played);
