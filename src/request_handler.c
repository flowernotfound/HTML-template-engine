#include "request_handler.h"
#include "html_serve.h"

void handle_client(int client_socket) {
  char buffer[BUFFER_SIZE] = {0};
  ssize_t received = read(client_socket, buffer, BUFFER_SIZE - 1);
  if (received <= 0) {
    perror("[ERROR] Failed to read from client");
    return;
  }

  printf("[DEBUG] Received request:\n%.*s\n", (int)received, buffer);

  char method[8], path[256];
  if (sscanf(buffer, "%7s %255s", method, path) != 2) {
    fprintf(stderr, "[ERROR] Failed to parse request line\n");
    return;
  }

  if (strcmp(method, "GET") != 0) {
    const char *error = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
    write(client_socket, error, strlen(error));
    return;
  }

  const char *filepath = (strcmp(path, "/") == 0) ? "index.html" : path + 1;

  size_t body_len = 0;
  char *html = serve_html(filepath, &body_len);

  if (html) {
    char header[256];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n\r\n",
             body_len);
    write(client_socket, header, strlen(header));
    write(client_socket, html, body_len);
    free(html);
  } else {
    const char *error = "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: text/plain\r\n"
                        "Connection: close\r\n\r\n"
                        "File not found.\n";
    write(client_socket, error, strlen(error));
  }

  shutdown(client_socket, SHUT_WR);
  close(client_socket);
}
