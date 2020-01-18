# include <stdio.h>
# include "networking.h"
# include "play_song.h"




void empty_string(char * buffer){
    int c = 0;
    while (buffer[c] != '\0') {
        buffer[c] = '\0';
        c++;
    }
}


int client(char * ip) {
  int server_socket;
  char buffer[BUFFER_SIZE];

  server_socket = client_setup(ip);
  clear();

  printf("%s\n", ip);
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
    //empty_string(buffer);
    *buffer = '\0';
    read(server_socket, buffer, sizeof(buffer));
    printf("[%s]\n", buffer);
  }
}
