#include "networking.h"
#include "server.h"

char * random_song() {
  return "songs/americanpie.wav";
}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
  close(client_socket);
  exit(0);
}

int server() {
  char buffer[100];
  int number_connections = 2;
  int f;
  int listen_socket;
  listen_socket = server_setup();

  while (1) {
    // clear();
    // child
    printf("Waiting for connections...\n");

    int counter = 0;
    while (counter < number_connections) {
        int client_socket = server_connect(listen_socket);
        f = fork();
        if (f == 0)
            subserver(client_socket);
        else
            counter++;
            close(client_socket);
    }

    f = fork();
    if (!f) {
        execlp("aplay", "aplay", random_song(), NULL);
    }
    else {
        while (1) {
        printf("Blocking\n");
        fgets(buffer, 100, stdin);
        printf("%s\n", buffer);
      }
    }
  }
}
