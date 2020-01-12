# include "networking.h"

int client(char * ip) {
  int server_socket;
  char buffer[BUFFER_SIZE];

  server_socket = client_setup(ip);

  printf("%s\n", ip);
  while (1) {
    printf("enter data: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
  }
}
