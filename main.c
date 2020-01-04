# include <stdio.h>
# include <string.h>
# include "executes.h"

int main() {
  char buffer[100];

  printf("Hello, this is our final project! Type 'exit' to exit\n"
         "Note: to run our project you will need to install mpg321\n"
         "by typing 'sudo apt install mpg321' into your linux terminal\n");

  while (1) {
    printf("Your options are:\n1: play song\n");

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
