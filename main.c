# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include "play_song.h"
# include "server.h"
# include "client.h"

int main() {
  clear();

  char buffer[100];

  printf("Hello, this is our music player final project! Type 'exit' to exit.\n\n");

  while (1) {
    printf("Your options are:\n1: Enter the music player\n2: Be the server\n3: Be the client\n\n");

    fgets(buffer, 100, stdin);
    buffer[strlen(buffer) - 1] = 0;

    if (strcmp(buffer, "exit") == 0) {
      clear();
      return 0;
    }

    if      (strcmp(buffer, "1") == 0) play_song();
    else if (strcmp(buffer, "2") == 0) server();
    else if (strcmp(buffer, "3") == 0) client();

    clear();
  }
}
