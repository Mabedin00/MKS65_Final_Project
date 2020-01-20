#include "networking.h"

static int init_server();
void sighandler();
void sighandler_2();
void sighandler_3();
void sighandler_4();
int random_int(int min, int max);
int server();
static int run_server_code();
char * random_song();
static int update_song();
void subserver(int client_socket, char ** songs_to_be_played);
int is_duplicate(char ** songs_to_be_played, int index);
int max_songs();
int ask_for_num_songs();
int ask_for_num_players();
