# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include "executes.h"

int main() {
  int f = fork();
  if (!f) execlp("clear", "clear", NULL);
  waitpid(f, NULL, 0);

  char buffer[100];

  printf("Hello, this is our music player final project! Type 'exit' to exit\n\n");

  while (1) {
    printf("Your options are:\n1: play song\n\n");

    fgets(buffer, 100, stdin);
    buffer[strlen(buffer) - 1] = 0;

    if (strcmp(buffer, "exit") == 0) {
      return 0;
    }

    if (strcmp(buffer, "1") == 0) {
      play_song(buffer);
    }
  }
}
