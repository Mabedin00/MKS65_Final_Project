# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>

void execute(char * program, char * argument) {
  int f = fork();
  if (!f) execlp(program, program, argument, NULL);
  waitpid(f, NULL, 0);
}

void clear() {
  execute("clear", NULL);
}

void play_song (char * command) {
  char buffer[100];

  clear();
  printf("Hello, you have entered the playing song interface\n\n");

  while (1) {
    printf("Type the name of a song to play it\n"
           "Type 'list all' to see a listing of all of our songs\n"
           "Type 'return' to return to the main selection screen\n\n");
    fgets(buffer, 100, stdin);
    buffer[strlen(buffer) - 1] = 0;

    clear();

    if (strcmp(buffer, "return") == 0) {
      execute("killall", "aplay");
      return;
    }

    else if (strcmp(buffer, "terminate") == 0) {
      execute("killall", "aplay");
    }

    else if (strcmp(buffer, "list all") == 0) {
      printf("This is what we've got:\n\n");
      execute("ls", "songs");
      printf("\n\n");
    }

    else {
      printf("Attempting to play... %s\n\n", buffer);
      printf("Type 'terminate' to end currently playing song\n");
      if (!fork()) {
        char song_path[100] = "songs/";
        strcat(song_path, buffer);
        execlp("aplay", "aplay", song_path, NULL);
      }
    }
  }
}
