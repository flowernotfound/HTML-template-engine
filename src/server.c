#include "server.h"

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = initialize_server(&address);
    if (server_fd == -1)
	{
        return (EXIT_FAILURE);
    }
    printf("Server listening on port 8080...\n");
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0)
	{
        perror("accept failed");
        close(server_fd);
        return (EXIT_FAILURE);
    }
    const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "Hello World!\n";
    write(new_socket, response, strlen(response));
    close(new_socket);
    close(server_fd);
    return (0);
}
