#include "networking.h"

void subserver(int client_socket, char ** songs_to_be_played, char ** incorrect_options);
void sighandler();
void sighandler_2();
void sighandler_3();
void sighandler_4();
int random_int(int min, int max);
int server();
int is_duplicate(char ** songs_to_be_played, int start, int index);
int max_songs();
int ask_for_num_songs();
int ask_for_num_players();
static int init_server();
static int run_server_code();
static int update_song();
static int return_to_main_page();
char * random_song();
