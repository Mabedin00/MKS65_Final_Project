#include "networking.h"

void sighandler();
void sighandler_2();
void sighandler_3();
int random_int(int min, int max);
int server();
char * random_song();
void subserver(int client_socket, char ** songs_to_be_played);
int is_duplicate(char ** songs_to_be_played, int index);
int max_songs();
