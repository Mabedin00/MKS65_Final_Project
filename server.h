#include "networking.h"

int server(char * num_songs);
char * random_song();
void subserver(int client_socket, char * song_to_be_played);
