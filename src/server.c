#include "server.h"
#include "html_serve.h"
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

    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("accept failed");
        close(server_fd);
        return EXIT_FAILURE;
    }

    size_t body_len = 0;
    char* html = serve_html("index.html", &body_len);

    if (html)
    {
        printf("[LOG] HTML loaded successfully. Size: %zu bytes\n", body_len);
        printf("[LOG] First 80 chars: %.80s\n", html);

        char header[256];
        snprintf(header, sizeof(header),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %zu\r\n"
                "Connection: close\r\n\r\n",
                body_len);

        ssize_t sent;

        sent = write(new_socket, header, strlen(header));
        if (sent == -1) {
            perror("[ERROR] write(header) failed");
        } else {
            printf("[LOG] Header sent: %zd bytes\n", sent);
        }

        sent = write(new_socket, html, body_len);
        if (sent == -1) {
            perror("[ERROR] write(body) failed");
        } else {
            printf("[LOG] Body sent: %zd bytes\n", sent);
        }

        free(html);
    } else {
        perror("[ERROR] Failed to load HTML");
        const char* error =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "File not found.\n";
        write(new_socket, error, strlen(error));
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
