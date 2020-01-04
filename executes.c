# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>

void play_song (char * command) {
  char buffer[100];

  printf("Hello, you have entered the playing song interface\n"
          "Type the song that you would like to play\n"
          "Type 'list all' to see a listing of all of our songs\n"
          "Type 'return' to return to the main selection screen\n");

  while (1) {
    fgets(buffer, 100, stdin);
    buffer[strlen(buffer) - 1] = 0;

    if (strcmp(buffer, "return") == 0) {
      exit(0);
    }

    if (strcmp(buffer, "list all") == 0) {
      printf("This is what we've got:\n");
    }

    printf("Attempting to play... %s\n", buffer);
    if (!fork()) execlp("mpg123", "mpg123", "songs/American_Pie.mp3", NULL);




  }
  // if (fork() == 0) {
	// 	if (execvp(args[0], args) == -1)
	// 		exit(-1);
	// }
}
