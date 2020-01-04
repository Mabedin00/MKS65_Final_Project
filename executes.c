# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>

void play_song (char * command) {
  int w;
  char buffer[100];

  printf("Hello, you have entered the playing song interface\n\n");

  while (1) {
    printf("Type the name of a song to play it\n"
           "Type 'list all' to see a listing of all of our songs\n"
           "Type 'return' to return to the main selection screen\n\n");
    fgets(buffer, 100, stdin);
    buffer[strlen(buffer) - 1] = 0;

    if (strcmp(buffer, "return") == 0) {
      if (!fork()) execlp("killall", "killall", "aplay", NULL);
      wait(&w);
      return;
    }

    else if (strcmp(buffer, "terminate") == 0) {
      if (!fork()) execlp("killall", "killall", "aplay", NULL);
      wait(&w);
    }

    else if (strcmp(buffer, "list all") == 0) {
      printf("This is what we've got:\n\n");
      if (!fork()) execlp("ls", "ls", "songs", NULL);
      wait(&w);
      printf("\n\n");
    }

    else {
      printf("Attempting to play... %s\n", buffer);
      printf("Type 'terminate' to end currently playing song\n");
      if (!fork()) {
        char song_path[100] = "songs/";
        strcat(song_path, buffer);
        execlp("aplay", "aplay", song_path, NULL);
      }
    }
  }
}
