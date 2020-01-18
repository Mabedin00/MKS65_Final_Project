# include <stdio.h>
# include "networking.h"
# include "play_song.h"

int client() {
  int server_socket;
  char buffer[BUFFER_SIZE];

  char ip[BUFFER_SIZE];
  //fgets(ip, BUFFER_SIZE, stdin);
  strcpy(ip, "149.89.150.103"); // for testing, change later
  server_socket = client_setup(ip);

  clear();

  printf("%s\n", ip);
  printf("Waiting for more players\n");

  read(server_socket, buffer, BUFFER_SIZE);
  printf("Received from subserver: [%s]\n", buffer);
  printf("Game starting\n");

  while (1) {
    printf("Guess a song: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = '\0';

    // removing all spaces
    char format[BUFFER_SIZE];
    int c = 0;
    int b = 0;
    int a = 0;
    while (buffer[c] != '\0') {
      if (buffer[c] == ' ') {
        b = c + 1;
        if (buffer[b] == ' ' && buffer[b] != '\0') {
          if (buffer[b] == ' ') {
            c++;
          }
          b++;
        }
      }
      format[a] = buffer[c];
      c++;
      a++;
    }

    //printf("%s", format);

    write(server_socket, buffer, sizeof(buffer));
    *buffer = '\0';
    read(server_socket, buffer, sizeof(buffer));
    printf("[%s]\n", buffer);
  }
}
