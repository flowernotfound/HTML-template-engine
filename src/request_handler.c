#include "request_handler.h"
#include "html_serve.h"
#include "template.h"

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

  const char *filepath =
      (strcmp(path, "/") == 0) ? "template_conditional.html" : path + 1;

  size_t body_len = 0;
  char *html = serve_html(filepath, &body_len);

  if (html) {
    printf("[DEBUG] Original HTML loaded, processing template...\n");

    const char *keys[] = {"site_name", "is_logged_in", "user", "role"};
    const char *values[] = {"C Template Engine", "", "Dexter",
                            "Senior Developer"};
    // const char *values[] = {"C Template Engine", "1", "Dexter",
    //                         "Senior Developer"};
    const char *loop_key = "item";
    const char *loop_values[] = {"Item 1", "Item 2", "Item 3"};

    char *processed_html =
        process_template(html, keys, values, 4, loop_key, loop_values, 3);

    if (processed_html) {
      printf("[DEBUG] Template processing completed successfully\n");

      size_t processed_len = strlen(processed_html);

      char header[256];
      snprintf(header, sizeof(header),
               "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Content-Length: %zu\r\n"
               "Connection: close\r\n\r\n",
               processed_len);

      write(client_socket, header, strlen(header));
      write(client_socket, processed_html, processed_len);

      free(processed_html);
    } else {
      printf("[ERROR] Template processing failed\n");
      const char *error = "HTTP/1.1 500 Internal Server Error\r\n"
                          "Content-Type: text/plain\r\n"
                          "Connection: close\r\n\r\n"
                          "Template processing failed.\n";
      write(client_socket, error, strlen(error));
    }

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
