#include "server.h"
#include "html_serve.h"
#include "request_handler.h"
#include <string.h>

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  server_fd = initialize_server(&address);
  if (server_fd == -1) {
    return EXIT_FAILURE;
  }

  printf("Server listening on port 8080...\n");

  while (1) {
    new_socket =
        accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
      perror("accept failed");
      continue;
    }

    printf("[LOG] Client connected.\n");
    handle_client(new_socket);
  }

  close(server_fd);
  return 0;
}
